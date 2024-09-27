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
    "DO",      // 0x0A
    "DO_1",    // 0x0B
    "DO_2",    // 0x0C
    "DO_3",    // 0x0D
    "DO_4",    // 0x0E
    "DO_5",    // 0x0F
    "DO_6",    // 0x10
    "DO_7",    // 0x11
    "DO_8",    // 0x12
    "DO_9",    // 0x13
    "DO_10",   // 0x14
    "DI_1",    // 0x15
    "DI_2",    // 0x16
    "DI_3",    // 0x17
    "DI_4",    // 0x18
    "DI_5",    // 0x19
    "DI_6",    // 0x1A
    "DI_7",    // 0x1B
    "DI_8",    // 0x1C
    "DI_9",    // 0x1D
    "DI_10",   // 0x1E
};

// 函数声明
void print_data_with_device(const char *label, int device_id, const unsigned char *data, size_t len);
void print_result(int device_id, int success, int iteration, size_t sent_len, size_t recv_len);
int send_and_receive(int client_fd, int device_id, int expected_sender_id, const unsigned char *data_to_send, size_t data_len);

void print_result(int device_id, int success, int iteration, size_t sent_len, size_t recv_len)
{
    const char *device_name = device_names[device_id];

    // 使用 ANSI 转义码进行颜色输出
    const char *success_color = "\033[32m"; // 绿色
    const char *failure_color = "\033[31m"; // 红色
    const char *reset_color = "\033[0m";    // 重置颜色

    if (success)
    {
        printf("%sIteration %d: [%s] (Device %02X) communication successful. Sent %zu bytes, Received %zu bytes.%s\n",
               success_color, iteration, device_name, device_id, sent_len, recv_len, reset_color);
    }
    else
    {
        printf("%sIteration %d: [%s] (Device %02X) communication failed. Sent %zu bytes, Received %zu bytes.%s\n",
               failure_color, iteration, device_name, device_id, sent_len, recv_len, reset_color);
    }
}

void print_data_with_device(const char *label, int device_id, const unsigned char *data, size_t len)
{
    // printf("%s (Device %02X): ", label, device_id);
    printf("%02X ", device_id);
    for (size_t i = 0; i < len; ++i)
    {
        printf("%02X ", data[i]);
    }
    printf("\n");
}

/**
 * @brief 取反
 *
 * @param value
 * @return unsigned char
 */
unsigned char toggle(unsigned char value)
{
    return value ^ 0x01;
}

// 发送和接收数据的封装函数
int send_and_receive(int client_fd, int device_id, int expected_sender_id, const unsigned char *data_to_send, size_t data_len)
{
    if (send_device_data(client_fd, device_id, data_to_send, data_len) < 0)
    {
        log_error("Failed to send data from Device %02X", device_id);
        return 0;
    }

    // const char *device_name = device_names[device_id];
    const char *device_name = (device_id < sizeof(device_names) / sizeof(device_names[0])) ? device_names[device_id] : "Unknown";

    printf("\033[33mSending  data [%s]: \033[0m", device_name); // 黄色文字表示等待状态

    print_data_with_device(" ", device_id, data_to_send, data_len);

    unsigned char recv_buffer[256];
    int recv_len = safe_recv(client_fd, recv_buffer, sizeof(recv_buffer), 0);
    if (recv_len > 0)
    {
        const char *recv_device_name = device_names[recv_buffer[0]];
        printf("Received data [%s]: ", recv_device_name);
        for (size_t j = 0; j < recv_len; ++j)
        {
            printf("%02X ", recv_buffer[j]);
        }
        printf("\n");

        // 校验接收到的设备ID是否是期望的发送者
        if (recv_buffer[0] != expected_sender_id)
        {
            log_error("Received data from unexpected device. Expected: %02X, but got: %02X", expected_sender_id, recv_buffer[0]);
            return 0; // 校验失败
        }

        // 校验数据长度是否与发送数据一致
        if (recv_len != data_len + 1)
        {
            log_error("Data length mismatch. Expected: %zu bytes, but got: %d bytes", data_len + 1, recv_len);
            return 0; // 长度不匹配
        }

        unsigned char temp[1];
        if (device_id >= DEVICE_DO_1)
        {
            for (size_t i = 0; i < data_len; i++)
            {
                temp[i] = toggle(recv_buffer[i + 1]); // 对接收到的数据进行取反
            }
        }
        else
        {
            memcpy(temp, data_to_send, data_len); // 直接复制
        }

        // 校验数据内容是否一致
        if (memcmp(data_to_send, temp, data_len) == 0)
        {
            return 1; // 校验成功，数据一致
        }
        else
        {
            log_error("Data mismatch between sent and received data");
            return 0; // 校验失败，数据不一致
        }

        // 数据校验成功
        return 1;
    }

    log_error("No data received from Device %02X", expected_sender_id);
    return 0; // 未收到数据
}

// 修改 send_test_data_ndev 函数，传递预期的发送方设备ID
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
    const unsigned char rs485_data_3_to_4[] = {0x49, 0x4A, 0x4B, 0x4C, 0x4D};
    const unsigned char rs485_data_4_to_3[] = {0x4E, 0x4F, 0x50, 0x51, 0x52};
    const unsigned char rs232_data_1[] = {0x55, 0x66, 0x6A, 0x6B};
    const unsigned char rs232_data_2[] = {0x77, 0x88, 0x8C, 0x8D};
    const unsigned char can_data_1_to_2[] = {0xCC, 0xCD, 0xCE};
    const unsigned char can_data_2_to_1[] = {0xDD, 0xDE, 0xDF};

    // unsigned char do_data[10] = {0x01, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01};

    unsigned char do_single_high_data[10] = {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}; // DI1 - DO10

    unsigned char do_single_low_data[10] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // DI1 - DO10

    int iteration = 0;

    while (1) // 死循环
    {
        iteration++;
        printf("Starting iteration %d...\n", iteration);

        // RS485 交叉互测
        print_result(DEVICE_RS485_1, send_and_receive(client_fd, DEVICE_RS485_1, DEVICE_RS485_2, rs485_data_1_to_2, sizeof(rs485_data_1_to_2)), iteration, sizeof(rs485_data_1_to_2), sizeof(rs485_data_1_to_2));
        print_result(DEVICE_RS485_2, send_and_receive(client_fd, DEVICE_RS485_2, DEVICE_RS485_1, rs485_data_2_to_1, sizeof(rs485_data_2_to_1)), iteration, sizeof(rs485_data_2_to_1), sizeof(rs485_data_2_to_1));
        print_result(DEVICE_RS485_3, send_and_receive(client_fd, DEVICE_RS485_3, DEVICE_RS485_4, rs485_data_3_to_4, sizeof(rs485_data_3_to_4)), iteration, sizeof(rs485_data_3_to_4), sizeof(rs485_data_3_to_4));
        print_result(DEVICE_RS485_4, send_and_receive(client_fd, DEVICE_RS485_4, DEVICE_RS485_3, rs485_data_4_to_3, sizeof(rs485_data_4_to_3)), iteration, sizeof(rs485_data_4_to_3), sizeof(rs485_data_4_to_3));
        printf("\n"); // 换行

        // RS232 自发自测
        print_result(DEVICE_RS232_1, send_and_receive(client_fd, DEVICE_RS232_1, DEVICE_RS232_1, rs232_data_1, sizeof(rs232_data_1)), iteration, sizeof(rs232_data_1), sizeof(rs232_data_1));
        print_result(DEVICE_RS232_2, send_and_receive(client_fd, DEVICE_RS232_2, DEVICE_RS232_2, rs232_data_2, sizeof(rs232_data_2)), iteration, sizeof(rs232_data_2), sizeof(rs232_data_2));
        printf("\n"); // 换行

        // CAN 交叉互测
        print_result(DEVICE_CAN_1, send_and_receive(client_fd, DEVICE_CAN_1, DEVICE_CAN_2, can_data_1_to_2, sizeof(can_data_1_to_2)), iteration, sizeof(can_data_1_to_2), sizeof(can_data_1_to_2));
        print_result(DEVICE_CAN_2, send_and_receive(client_fd, DEVICE_CAN_2, DEVICE_CAN_1, can_data_2_to_1, sizeof(can_data_2_to_1)), iteration, sizeof(can_data_2_to_1), sizeof(can_data_2_to_1));
        printf("\n"); // 换行

        // DO 自发数据
        // print_result(DEVICE_DO, send_and_receive(client_fd, DEVICE_DO, DEVICE_DI, do_data, sizeof(do_data)), iteration, sizeof(do_data), sizeof(do_data));

        // 低电平循环
        printf("Starting high level iteration for DO devices...\n");
        for (size_t i = 0; i < sizeof(device_do_ids) / sizeof(device_do_ids[0]); i++)
        {
            unsigned char temp[1] = {do_single_high_data[i]};
            print_result(device_do_ids[i], send_and_receive(client_fd, device_do_ids[i], device_di_ids[i], temp, sizeof(temp)), iteration, sizeof(temp), sizeof(temp));
        }

        // 高电平循环
        printf("\nStarting low level iteration for DO devices...\n");
        for (size_t i = 0; i < sizeof(device_do_ids) / sizeof(device_do_ids[0]); i++)
        {
            unsigned char temp[1] = {do_single_low_data[i]};
            print_result(device_do_ids[i], send_and_receive(client_fd, device_do_ids[i], device_di_ids[i], temp, sizeof(temp)), iteration, sizeof(temp), sizeof(temp));
        }

        sleep(1); // 暂停一秒
    }
}
