#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include "send_device_data.h"
#include "logger.h"
#include "di_data.h"
#include "common.h"
#include "safe_recv_send.h"

// 设备名称映射
const char *device_names[] = {
    "Unknown", // 0x00
    "RS485_1", // 0x01
    "RS485_2", // 0x02
    "RS485_3", // 0x03
    "RS485_4", // 0x04
    "RS232_1", // 0x05
    "RS232_2", // 0x06
    "CAN_1",   // 0x07
    "CAN_2",   // 0x08
    "DI",      // 0x09
    "DO"       // 0x0A
};

// 函数声明
void print_data_with_device(const char *label, int device_id, const unsigned char *data, size_t len);
void print_result(int device_id, int success, int iteration, size_t sent_len, size_t recv_len);
int send_and_receive(int client_fd, int device_id, const unsigned char *data_to_send, size_t data_len);

void print_result(int device_id, int success, int iteration, size_t sent_len, size_t recv_len)
{
    const char *device_name = device_names[device_id];
    if (success)
    {
        printf("Iteration %d: Device %02X (%s) communication successful. Sent %zu bytes, Received %zu bytes.\n",
               iteration, device_id, device_name, sent_len, recv_len);
    }
    else
    {
        printf("Iteration %d: Device %02X (%s) communication failed. Sent %zu bytes, Received %zu bytes.\n",
               iteration, device_id, device_name, sent_len, recv_len);
    }
}

void print_data_with_device(const char *label, int device_id, const unsigned char *data, size_t len)
{
    printf("%s (Device %02X): ", label, device_id);
    for (size_t i = 0; i < len; ++i)
    {
        printf("%02X ", data[i]);
    }
    printf("\n");
}

// 发送和接收数据的封装函数
int send_and_receive(int client_fd, int device_id, const unsigned char *data_to_send, size_t data_len)
{
    if (send_device_data(client_fd, device_id, data_to_send, data_len) < 0)
    {
        log_error("Failed to send data from Device %02X", device_id);
        return 0;
    }

    print_data_with_device("Sent data", device_id, data_to_send, data_len);
    unsigned char recv_buffer[256];
    int recv_len = safe_recv(client_fd, recv_buffer, sizeof(recv_buffer), 0);
    if (recv_len > 0)
    {
        // print_data_with_device("Received data", device_id, recv_buffer + 1, recv_len - 1);
        printf("Complete received data: ");
        for (size_t j = 0; j < recv_len; ++j)
        {
            printf("%02X ", recv_buffer[j]);
        }
        printf("\n");
        return memcmp(data_to_send, recv_buffer + 1, data_len) == 0; // 简化数据校验
    }
    return 0; // 未收到数据
}

// 发送测试数据
void send_test_data_ndev(int client_fd)
{
    const unsigned char command[] = {
        0xAA,
        0xAA,
        0xAA,
        0xAA,
        0xAA,
    };

    // 发送指令
    if (safe_send(client_fd, command, sizeof(command), 0) < 0)
    {
        log_error("Failed to send test command");
    }

    printf("Sent command: ");
    for (size_t i = 0; i < sizeof(command); ++i)
    {
        printf("%02X ", command[i]);
    }
    printf("\n");

    const unsigned char rs485_data_1_to_2[] = {0x41, 0x42, 0x45, 0x46, 0x47};
    const unsigned char rs485_data_2_to_1[] = {0x43, 0x44, 0x48, 0x49, 0x4A};
    const unsigned char rs232_data_1[] = {0x55, 0x66, 0x6A, 0x6B};
    const unsigned char rs232_data_2[] = {0x77, 0x88, 0x8C, 0x8D};
    const unsigned char can_data_1_to_2[] = {0xCC, 0xCD, 0xCE};
    const unsigned char can_data_2_to_1[] = {0xDD, 0xDE, 0xDF};
    const unsigned char do_data[] = {0xAA, 0xAB};

    // usleep(50000); // Delay after sending data

    int iteration = 0;

    while (1) // 死循环
    {
        iteration++;
        printf("Starting iteration %d...\n", iteration);

        // RS485 交叉互测
        print_result(DEVICE_RS485_1, send_and_receive(client_fd, DEVICE_RS485_1, rs485_data_1_to_2, sizeof(rs485_data_1_to_2)), iteration, sizeof(rs485_data_1_to_2), sizeof(rs485_data_1_to_2));
        print_result(DEVICE_RS485_2, send_and_receive(client_fd, DEVICE_RS485_2, rs485_data_2_to_1, sizeof(rs485_data_2_to_1)), iteration, sizeof(rs485_data_2_to_1), sizeof(rs485_data_2_to_1));
        printf("\n"); // 换行

        // RS232 自发自测
        print_result(DEVICE_RS232_1, send_and_receive(client_fd, DEVICE_RS232_1, rs232_data_1, sizeof(rs232_data_1)), iteration, sizeof(rs232_data_1), sizeof(rs232_data_1));
        print_result(DEVICE_RS232_2, send_and_receive(client_fd, DEVICE_RS232_2, rs232_data_2, sizeof(rs232_data_2)), iteration, sizeof(rs232_data_2), sizeof(rs232_data_2));
        printf("\n"); // 换行

        // CAN 交叉互测
        print_result(DEVICE_CAN_1, send_and_receive(client_fd, DEVICE_CAN_1, can_data_1_to_2, sizeof(can_data_1_to_2)), iteration, sizeof(can_data_1_to_2), sizeof(can_data_1_to_2));
        print_result(DEVICE_CAN_2, send_and_receive(client_fd, DEVICE_CAN_2, can_data_2_to_1, sizeof(can_data_2_to_1)), iteration, sizeof(can_data_2_to_1), sizeof(can_data_2_to_1));
        printf("\n"); // 换行

        // DO 自发数据
        print_result(DEVICE_DO, send_and_receive(client_fd, DEVICE_DO, do_data, sizeof(do_data)), iteration, sizeof(do_data), sizeof(do_data));

        sleep(1); // 暂停一秒
    }
}
