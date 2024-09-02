#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <pthread.h>
#include <ctype.h>
#include "logger.h"
#include "di_data.h"
#include "common.h"
#include "server.h"
#include "send_device_data.h"
#include "server_recv_send.h"
#include "send_test_data.h"

#define SEND_RETRY_LIMIT 3 // Max number of retries if sending fails

// Global variables to communicate between threads
typedef struct
{
    int client_fd;
    pthread_mutex_t mutex;
    char response[256];
} ThreadData;

void *recv_thread(void *arg);
void *send_thread(void *arg);

void server_recv_send(int client_fd)
{
    ThreadData thread_data;
    pthread_t recv_tid, send_tid;

    // Initialize thread data
    thread_data.client_fd = client_fd;
    pthread_mutex_init(&thread_data.mutex, NULL);

    // Create threads for receiving and sending
    pthread_create(&recv_tid, NULL, recv_thread, &thread_data);
    pthread_create(&send_tid, NULL, send_thread, &thread_data);

    // Wait for threads to finish
    pthread_join(recv_tid, NULL);
    pthread_join(send_tid, NULL);

    close(client_fd);
    pthread_mutex_destroy(&thread_data.mutex);
}

void *recv_thread(void *arg)
{
    ThreadData *data = (ThreadData *)arg;
    unsigned char buf[MAX_LINE];
    ssize_t bytes_received;
    DI_Data di_data;

    while (1)
    {
        memset(buf, 0, sizeof(buf));
        bytes_received = recv(data->client_fd, buf, MAX_LINE, 0);

        if (bytes_received <= 0)
        {
            log_error("Client disconnected or error occurred");
            break;
        }

        unsigned short device_id = buf[0];
        log_info("\nDevice ID: %02X", device_id);

        printf("Received data (raw): ");
        fwrite(buf, sizeof(char), bytes_received, stdout);

        printf("Received data (hex): ");
        print_hex(buf, bytes_received);

        // Process device DI data
        if (device_id == DEVICE_DI)
        {
            handle_device_DI(buf, bytes_received, &di_data);
        }
        else
        {
            // Process data for other devices
            printf("Data following device ID (string): ");
            size_t data_length = bytes_received - 1;
            buf[1 + data_length] = (data_length < sizeof(buf) - 1) ? '\0' : buf[sizeof(buf) - 1];
            printf("%s\n", (char *)(buf + 1));
        }

        // Prepare the response
        pthread_mutex_lock(&data->mutex);
        snprintf(data->response, sizeof(data->response), "Message received and processed.");
        pthread_mutex_unlock(&data->mutex);

        usleep(5000); // 5ms
    }

    return NULL;
}

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
            sscanf(hex_str + i, "%2x", &value);
            buffer[index++] = (unsigned char)value;
        }
    }
    return index;
}

void *send_thread(void *arg)
{
    ThreadData *data = (ThreadData *)arg;
    char command_buffer[256];
    FILE *command_file;

    while (1)
    {
        pthread_mutex_lock(&data->mutex);

        // 打开命令文件进行读取
        command_file = fopen(COMMAND_FILE, "r+");
        if (command_file != NULL)
        {
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

                printf("command_buffer raw=%s\n", command_buffer);
                printf("device_id_str raw=%s\n", device_id_str);

                if (device_id_str && command_data)
                {
                    unsigned char device_id = (unsigned char)strtol(device_id_str, NULL, 16);

                    // 将十六进制字符串命令数据转换为字节数组
                    unsigned char data_buffer[256];
                    size_t data_length = hex_string_to_bytes(command_data, data_buffer, sizeof(data_buffer));

                    // 发送命令到客户端
                    if (send_device_data(data->client_fd, device_id, data_buffer, data_length) == -1)
                    {
                        perror("Failed to send command to client");
                    }
                    else
                    {
                        printf("Sent command to client: %s\n", command_data);
                    }
                }
                else
                {
                    fprintf(stderr, "Invalid command format: %s\n", command_buffer);
                }
            }

            // 清空文件内容
            freopen(COMMAND_FILE, "w", command_file); // 重新以 "w" 模式打开文件以清空内容
            fclose(command_file);
        }
        else
        {
            perror("Failed to open command file for reading");
        }

        if (MODE_SEND_TEST == 1)
        {
            // send_test_data(data->client_fd); // 你需要定义这个函数
        }

        pthread_mutex_unlock(&data->mutex);

        usleep(10000); // 每 10ms 检查一次
    }

    return NULL;
}