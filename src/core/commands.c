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

// 转换十六进制字符串到字节数组
size_t hex_string_to_bytes(const char *hex_str, unsigned char *buffer, size_t buffer_size)
{
    size_t length = strlen(hex_str);
    size_t index = 0;

    for (size_t i = 0; i < length && index < buffer_size; i += 3)
    { // 每两个字符为一个字节，中间有空格
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

        // 解析设备ID和命令
        char *device_id_str = strtok(command_buffer, " ");
        char *command_data = strtok(NULL, "");

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
                if (send_device_data(data->client_fd, device_id, data_buffer, data_length) == -1)
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
            fprintf(stderr, "Error: Invalid command format in file: '%s'\n", command_buffer);
        }
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