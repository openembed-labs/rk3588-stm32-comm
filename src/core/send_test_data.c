#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include "logger.h"
#include "di_data.h"
#include "common.h"

#define DELAY_MICROSECOND 500000 // 0.5s 每次发送数据后的延迟时间，单位为微秒

// 发送测试数据函数
void send_test_data(int client_fd)
{
    // 示例数据
    const unsigned char rs485_data[] = {0x01, 0x41, 0x42}; // 示例数据
    const unsigned char rs232_data[] = {0x11, 0x22, 0x33}; // 示例数据
    const unsigned char can_data[] = {0xAA, 0xBB};         // 示例数据

    // 设备数据示例
    DI_Data do_data = {DEVICE_DO, 0x01, 0x02, {0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00}};

    unsigned char buffer[256];
    size_t buffer_len;
    ssize_t bytes_sent;

    // 发送RS485设备数据
    for (int i = DEVICE_RS485_1; i <= DEVICE_RS485_4; ++i)
    {
        buffer[0] = i;
        memcpy(buffer + 1, rs485_data, sizeof(rs485_data));
        buffer_len = 1 + sizeof(rs485_data);
        bytes_sent = send(client_fd, buffer, buffer_len, 0);
        if (bytes_sent < 0)
            perror("Failed to send RS485 data");
        else
            printf("Sent RS485 data to device %02X\n", i);
        usleep(DELAY_MICROSECOND);
    }

    // 发送RS232设备数据
    for (int i = DEVICE_RS232_1; i <= DEVICE_RS232_2; ++i)
    {
        buffer[0] = i;
        memcpy(buffer + 1, rs232_data, sizeof(rs232_data));
        buffer_len = 1 + sizeof(rs232_data);
        bytes_sent = send(client_fd, buffer, buffer_len, 0);
        if (bytes_sent < 0)
            perror("Failed to send RS232 data");
        else
            printf("Sent RS232 data to device %02X\n", i);
        usleep(DELAY_MICROSECOND);
    }

    // 发送CAN设备数据
    for (int i = DEVICE_CAN_1; i <= DEVICE_CAN_2; ++i)
    {
        buffer[0] = i;
        memcpy(buffer + 1, can_data, sizeof(can_data));
        buffer_len = 1 + sizeof(can_data);
        bytes_sent = send(client_fd, buffer, buffer_len, 0);
        if (bytes_sent < 0)
            perror("Failed to send CAN data");
        else
            printf("Sent CAN data to device %02X\n", i);

        usleep(DELAY_MICROSECOND);
    }

    // 处理DO设备
    buffer[0] = DEVICE_DO;
    encode_di_data(&do_data, buffer + 1);
    buffer_len = 1 + 3; // 1字节设备号 + 3字节数据
    bytes_sent = send(client_fd, buffer, buffer_len, 0);
    if (bytes_sent < 0)
        perror("Failed to send DO data");
    else
        printf("Sent DO data to device %02X\n", DEVICE_DO);

    usleep(DELAY_MICROSECOND);
}