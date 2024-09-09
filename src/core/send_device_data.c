#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include "send_device_data.h"
#include "logger.h"
#include "di_data.h"
#include "common.h"
#include "safe_recv_send.h"

#define SEND_RETRY_LIMIT 3 // Max number of retries if sending fails

int send_device_data(int client_fd, unsigned char device_id, const unsigned char *data, size_t data_length)
{
    unsigned char buffer[256];
    size_t buffer_len;
    ssize_t bytes_sent = 0;
    size_t total_sent = 0;
    int retries = 0;

    if (device_id == DEVICE_DO)
    {
        // Handle DEVICE_DO specifically
        DI_Data do_data;

        if (data_length < 11)
        {
            log_error("Insufficient data length for DEVICE_DO");
            return -1;
        }

        // Populate the DI_Data structure
        do_data.DI = data[0];                   // First byte for DI
        do_data.DI_9 = (data[1] & 0x01);        // Second byte's 1st bit for DI_9
        do_data.DI_10 = (data[1] & 0x02) >> 1;  // Second byte's 2nd bit for DI_10
        memcpy(do_data.DI_values, data + 2, 8); // Remaining 8 bytes for DI_values

        // Encode DI_Data into the buffer
        encode_di_data(&do_data, buffer + 1);
        buffer[0] = device_id;
        buffer_len = 1 + 3; // 1 byte for device_id + 3 bytes for encoded data
    }
    else
    {
        // Handle other devices
        buffer[0] = device_id;                 // Set device_id
        memcpy(buffer + 1, data, data_length); // Copy data into buffer
        buffer_len = 1 + data_length;
    }

    // Attempt to send the data
    while (total_sent < buffer_len)
    {
        bytes_sent = safe_send_interceptor(client_fd, buffer + total_sent, buffer_len - total_sent, 0);

        if (bytes_sent <= 0)
        {
            // perror("Failed to send data");
            if (++retries > SEND_RETRY_LIMIT)
            {
                log_error("Max retries reached. Aborting send.");
                return -1;
            }
            continue;
        }

        total_sent += bytes_sent;
    }

    // log_info("Sent %zu bytes to client successfully", total_sent);
    usleep(50000); // Delay after sending data
    return 0;
}

int send_device_data_pure(int client_fd, unsigned char device_id, const unsigned char *data, size_t data_length)
{
    unsigned char buffer[256];
    size_t buffer_len;
    ssize_t bytes_sent = 0;
    size_t total_sent = 0;
    int retries = 0;

    buffer[0] = device_id;                 // 设置设备号
    memcpy(buffer + 1, data, data_length); // 复制数据到缓冲区
    buffer_len = 1 + data_length;

    // 尝试发送数据
    while (total_sent < buffer_len)
    {
        bytes_sent = safe_send_interceptor(client_fd, buffer + total_sent, buffer_len - total_sent, 0);

        if (bytes_sent <= 0)
        {
            // perror("Failed to send data");
            if (++retries > SEND_RETRY_LIMIT)
            {
                log_error("Max retries reached. Aborting send.");
                return -1;
            }
            continue;
        }

        total_sent += bytes_sent;
    }

    // log_info("Sent %zu bytes to client successfully", total_sent);
    usleep(DELAY_MICROSECOND); // 发送后延迟
    return 0;
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
        bytes_sent = safe_send_interceptor(client_fd, buffer + total_sent, total_length - total_sent, 0);
        if (bytes_sent <= 0)
        {
            // 发送失败，可能需要重试
            // perror("Failed to send data");
            return;
        }
        total_sent += bytes_sent;
    }

    printf("Successful send %zu bite data\n", total_sent);
}