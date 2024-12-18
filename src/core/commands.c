#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <pthread.h>
#include "commands.h"
#include "common.h"
#include "server_recv_send.h"
#include "send_device_data.h"
#include "firmware.h"
#include "send_test_data.h"
#include "safe_recv_send.h"
#include "logger.h"

// 转换十六进制字符串到字节数组
size_t hex_string_to_bytes(const char *hex_str, unsigned char *buffer, size_t buffer_size)
{
    size_t length = strlen(hex_str);
    size_t index = 0;

    for (size_t i = 0; i < length && index < buffer_size; i += 3)
    {
        if (isxdigit(hex_str[i]) && isxdigit(hex_str[i + 1]))
        {
            unsigned int value;
            if (sscanf(hex_str + i, "%2x", &value) != 1)
            {
                fprintf(stderr, "Error: Failed to parse hex string '%s' at position %zu\n", hex_str, i);
                return 0;
            }
            buffer[index++] = (unsigned char)value;
        }
        else
        {
            fprintf(stderr, "Error: Invalid hex digit '%c%c' at position %zu\n", hex_str[i], hex_str[i + 1], i);
            return 0;
        }
    }
    return index;
}

// 处理特殊命令
void process_special_command(int client_fd, const char *command, ThreadData *data)
{
    // 检查是否是升级固件命令 "/upgrade <文件路径>"
    char *cmd = strtok(strdup(command), " ");
    if (strcmp(cmd, "/upgrade") == 0)
    {
        char *firmware_path = strtok(NULL, " "); // 获取文件路径
        if (firmware_path)
        {
            printf("Upgrade firmware command received. Path: %s\n", firmware_path);

            // 开始升级，设置标志
            // pthread_mutex_lock(&data->mutex);
            data->is_upgrading = 1; // 设置为正在升级
            // pthread_mutex_unlock(&data->mutex);

            // 调用发送固件的函数
            send_firmware(client_fd, firmware_path);

            // 升级完成，重置标志
            // pthread_mutex_lock(&data->mutex);
            data->is_upgrading = 0; // 结束升级
            // pthread_mutex_unlock(&data->mutex);
        }

        else
        {
            fprintf(stderr, "Error: No firmware path provided for upgrade command.\n");
        }
    }
    else if (strcmp(cmd, "/st") == 0)
    {
        printf("Send test command received.\n");
        // 发送十六进制指令，表示开始发送测试数据
        const unsigned char command[] = {
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
        }; // 示例指令

        if (safe_send(client_fd, command, sizeof(command), 0) < 0)
        {
            log_error("Failed to send test command");
            // return -1; // 标记失败
        }
        printf("Sent test command: ");
        print_hex(command, sizeof(command)); // 打印发送的指令
        send_test_data(client_fd);
    }

    else
    {
        fprintf(stderr, "Error: Unrecognized special command: %s\n", cmd);
    }
    free(cmd);
}

// 处理普通设备命令
void process_device_command(int client_fd, const char *device_id_str, const char *command_data)
{
    if (device_id_str && command_data)
    {
        unsigned char device_id = (unsigned char)strtol(device_id_str, NULL, 16);

        // 将十六进制字符串命令数据转换为字节数组
        unsigned char data_buffer[256];
        size_t data_length = hex_string_to_bytes(command_data, data_buffer, sizeof(data_buffer));

        if (data_length == 0)
        {
            fprintf(stderr, "Error: Failed to convert command data '%s' to bytes\n", command_data);
        }
        else
        {
            // 发送命令到客户端
            if (send_device_data(client_fd, device_id, data_buffer, data_length) == -1)
            {
                fprintf(stderr, "Error: Failed to send command to client (device_id: 0x%02X)\n", device_id);
            }
            else
            {
                printf("Sent command to client (device_id: 0x%02X): %s\n", device_id, command_data);
            }
        }
    }
    else
    {
        fprintf(stderr, "Error: Invalid command format. Device ID or command data missing.\n");
    }
}

// 解析并处理命令
void parse_and_process_command(int client_fd, const char *command_buffer, ThreadData *data)
{
    // 检查是否是特殊命令（以 "/" 开头）
    if (command_buffer[0] == '/')
    {
        // 处理特殊命令
        process_special_command(client_fd, command_buffer, data);
    }
    else
    {
        // 处理普通设备命令
        char *device_id_str = strtok(strdup(command_buffer), " ");
        char *command_data = strtok(NULL, "");
        process_device_command(client_fd, device_id_str, command_data);
        free(device_id_str); // 清理 strdup 创建的内存
    }
}

// 从文件中读取命令并处理
void process_command_file(ThreadData *data)
{
    char command_buffer[256];
    FILE *command_file;

    // 尝试以读写模式打开命令文件，如果文件不存在则创建文件
    command_file = fopen(COMMAND_FILE, "r+");
    if (command_file == NULL)
    {
        // 文件不存在，尝试以写入模式创建文件
        command_file = fopen(COMMAND_FILE, "w+");
        if (command_file == NULL)
        {
            perror("Error: Failed to open or create command file");
            return;
        }
    }

    // 读取并处理文件中的命令
    if (fgets(command_buffer, sizeof(command_buffer), command_file) != NULL)
    {
        // 去掉命令末尾的换行符
        size_t len = strlen(command_buffer);
        if (len > 0 && command_buffer[len - 1] == '\n')
        {
            command_buffer[len - 1] = '\0';
        }

        // 解析并处理命令
        parse_and_process_command(data->client_fd, command_buffer, data);
    }

    // 清空文件内容
    if (freopen(COMMAND_FILE, "w", command_file) == NULL)
    {
        perror("Error: Failed to clear command file");
        fclose(command_file); // Don't forget to close the original file if freopen fails
        return;
    }

    fclose(command_file);
}
