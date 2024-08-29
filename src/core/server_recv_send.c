#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <pthread.h>
#include "logger.h"
#include "di_data.h"
#include "common.h"
#include "server.h"
#include "server_recv_send.h"

#define SEND_RETRY_LIMIT 3 // Max number of retries if sending fails

// Global variables to communicate between threads
typedef struct
{
    int client_fd;
    pthread_mutex_t mutex;
    char response[256];
    int send_ready;
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
    thread_data.send_ready = 0;

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
        data->send_ready = 1;
        pthread_mutex_unlock(&data->mutex);

        usleep(5000); // 5ms
    }

    return NULL;
}

// 构造要发送的数据
void construct_and_send_data(int client_fd)
{
    // 示例数据：十六进制数据 {0x00, 0x41, 0x42}
    const unsigned char data[] = {0x00, 0x41, 0x42}; // 这是十六进制数据
    size_t data_length = sizeof(data);

    // 数据缓冲区，第一字节是设备号，后面是数据
    unsigned char buffer[256];

    // 设置设备号
    buffer[0] = DEVICE_DI;

    // 复制数据到缓冲区
    memcpy(buffer + 1, data, data_length);

    // 数据长度是设备号后面的长度
    size_t total_length = 1 + data_length;

    // 发送数据
    ssize_t bytes_sent = 0;
    ssize_t total_sent = 0;

    while (total_sent < total_length)
    {
        bytes_sent = send(client_fd, buffer + total_sent, total_length - total_sent, 0);
        if (bytes_sent <= 0)
        {
            // 发送失败，可能需要重试
            perror("Failed to send data");
            return;
        }
        total_sent += bytes_sent;
    }

    printf("Successful send %zu bite data\n", total_sent);
}

void *send_thread(void *arg)
{
    ThreadData *data = (ThreadData *)arg;

    while (1)
    {
        pthread_mutex_lock(&data->mutex);
        if (data->send_ready)
        {
            // 数据准备完毕，调用构造函数发送数据
            construct_and_send_data(data->client_fd);

            // 更新状态
            data->send_ready = 0;
            pthread_mutex_unlock(&data->mutex);
        }
        else
        {
            pthread_mutex_unlock(&data->mutex);
        }

        usleep(10000); // 每 10ms 检查一次
    }

    return NULL;
}
