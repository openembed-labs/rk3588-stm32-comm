#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include "send_test_data.h"
#include "send_device_data.h"
#include "logger.h"
#include "di_data.h"
#include "common.h"

int send_test_data(int client_fd)
{
    // 示例数据
    const unsigned char rs485_data[] = {0x01, 0x41, 0x42}; // 示例数据
    const unsigned char rs232_data[] = {0x11, 0x22, 0x33}; // 示例数据
    const unsigned char can_data[] = {0xAA, 0xBB};         // 示例数据

    // 设备数据示例
    DI_Data do_data = {DEVICE_DO, 0x01, 0x02, {0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00}};

    int result = 0;

    // 发送RS485设备数据
    for (int i = DEVICE_RS485_1; i <= DEVICE_RS485_4; ++i)
    {
        if (send_device_data(client_fd, i, rs485_data, sizeof(rs485_data)) < 0)
        {
            log_error("Failed to send RS485 data");
            result = -1; // 标记失败
        }
        else
        {
            printf("Sent RS485 data to device %02X\n", i);
        }
        usleep(DELAY_MICROSECOND);
    }

    // 发送RS232设备数据
    for (int i = DEVICE_RS232_1; i <= DEVICE_RS232_2; ++i)
    {
        if (send_device_data(client_fd, i, rs232_data, sizeof(rs232_data)) < 0)
        {
            log_error("Failed to send RS232 data");
            result = -1; // 标记失败
        }
        else
        {
            printf("Sent RS232 data to device %02X\n", i);
        }
        usleep(DELAY_MICROSECOND);
    }

    // 发送CAN设备数据，每个数据包必须是8个字节
    for (int i = DEVICE_CAN_1; i <= DEVICE_CAN_2; ++i)
    {
        unsigned char can_packet[8] = {0};

        // 填充CAN数据包，确保数据长度为8字节
        size_t can_data_len = sizeof(can_data);
        memcpy(can_packet, can_data, can_data_len);

        if (send_device_data(client_fd, i, can_packet, sizeof(can_packet)) < 0)
        {
            log_error("Failed to send CAN data");
            result = -1; // 标记失败
        }
        else
        {
            printf("Sent CAN data to device %02X\n", i);
        }
        usleep(DELAY_MICROSECOND);
    }

    // 处理DO设备
    unsigned char buffer[256];
    size_t buffer_len;
    int bytes_sent;

    buffer[0] = DEVICE_DO;
    encode_di_data(&do_data, buffer + 1);
    buffer_len = 1 + 3; // 1字节设备号 + 3字节编码数据
    bytes_sent = send(client_fd, buffer, buffer_len, 0);
    if (bytes_sent < 0)
    {
        perror("Failed to send DO data");
        result = -1; // 标记失败
    }
    else
    {
        printf("Sent DO data to device %02X\n", DEVICE_DO);
    }

    usleep(DELAY_MICROSECOND);

    return result;
}
