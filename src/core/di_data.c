#include "di_data.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include "logger.h"

// 实现 generate_di_data 函数
void generate_di_data(DI_Data *data)
{
    data->DI = 00;
    data->DI_9 = 9;
    data->DI_10 = 10;
    data->DI_values[0] = 1;
    data->DI_values[1] = 2;
    data->DI_values[2] = 3;
    data->DI_values[3] = 4;
    data->DI_values[4] = 5;
    data->DI_values[5] = 6;
    data->DI_values[6] = 7;
    data->DI_values[7] = 8;
}

// 实现 encode_di_data 函数
void encode_di_data(const DI_Data *data, unsigned char *buf)
{
    buf[0] = data->DI;
    buf[1] = data->DI_9 + (data->DI_10 << 1);
    buf[2] = data->DI_values[0] + (data->DI_values[1] << 1) + (data->DI_values[2] << 2) + (data->DI_values[3] << 3) + (data->DI_values[4] << 4) + (data->DI_values[5] << 5) + (data->DI_values[6] << 6) + (data->DI_values[7] << 7);
}

// 实现 decode_di_data 函数
void decode_di_data(const unsigned char *buf, DI_Data *data)
{
    data->DI = buf[0];
    data->DI_9 = buf[1] & 0x01;
    data->DI_10 = (buf[1] >> 1) & 0x01;
    data->DI_values[0] = buf[2] & 0x01;
    data->DI_values[1] = (buf[2] >> 1) & 0x01;
    data->DI_values[2] = (buf[2] >> 2) & 0x01;
    data->DI_values[3] = (buf[2] >> 3) & 0x01;
    data->DI_values[4] = (buf[2] >> 4) & 0x01;
    data->DI_values[5] = (buf[2] >> 5) & 0x01;
    data->DI_values[6] = (buf[2] >> 6) & 0x01;
    data->DI_values[7] = (buf[2] >> 7) & 0x01;
}

// 实现 send_di_data 函数
void send_di_data(int connection_fd, const DI_Data *data)
{
    unsigned char buf[3];
    encode_di_data(data, buf);
    ssize_t bytes_sent = send(connection_fd, buf, sizeof(buf), 0);
    if (bytes_sent == -1)
    {
        perror("Send error");
        // Handle error
    }
}

// 实现 receive_di_data 函数
void receive_di_data(int c_fd, DI_Data *di_data)
{
    // 解析收到的数据为 DI_Data 结构体
    unsigned char buf[3];
    ssize_t bytes_received = recv(c_fd, buf, sizeof(buf), 0);
    if (bytes_received > 0)
    {
        // 解析数据到 di_data 结构体
        // 根据协议将 buf 数据解析到 di_data 中
        di_data->DI = buf[0];
        di_data->DI_9 = (buf[1] & 0x01);
        di_data->DI_10 = (buf[1] >> 1) & 0x01;
        for (int i = 0; i < 8; i++)
        {
            di_data->DI_values[i] = (buf[2] >> i) & 0x01;
        }
    }
    else
    {
        log_error("Receive error or no data");
    }
}

// 处理DI的数据
void handle_device_DI(const unsigned char *buf, size_t bytes_received, DI_Data *di_data)
{
    // 数据以二进制格式打印
    printf("Data following device DI (binary): ");
    for (size_t i = 1; i < bytes_received; i++)
    {
        print_binary(buf[i]);
        printf(" ");
    }
    printf("\n");

    // 接收 DI 数据
    decode_di_data(buf, di_data);
    printf("Received DI Data:\n");
    printf("DI: %u\n", di_data->DI);
    printf("DI_9: %u, DI_10: %u\n", di_data->DI_9, di_data->DI_10);
    printf("DI values: %u %u %u %u %u %u %u %u\n",
           di_data->DI_values[0], di_data->DI_values[1], di_data->DI_values[2],
           di_data->DI_values[3], di_data->DI_values[4], di_data->DI_values[5],
           di_data->DI_values[6], di_data->DI_values[7]);
}
