#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "server.h"
#include "logger.h"
#include "di_data.h"
#include "common.h"

void run_server(const char *address, int port)
{
    int s_fd, c_fd;
    unsigned char buf[MAX_LINE];
    struct sockaddr_in server_addr, client_addr;
    socklen_t len;
    ssize_t bytes_received;

    DI_Data di_data;

    s_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (s_fd == -1)
    {
        log_error("Socket creation error");
        exit(1);
    }

    // 设置 SO_REUSEADDR 套接字选项
    int opt = 1;
    if (setsockopt(s_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
    {
        log_error("setsockopt error");
        close(s_fd);
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(address);
    server_addr.sin_port = htons(port);

    log_info("Server address: %s:%d", address, port);

    int status = bind(s_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (status == -1)
    {
        log_error("Bind error");
        close(s_fd);
        exit(1);
    }

    if (listen(s_fd, MAX_PADDING) == -1)
    {
        log_error("Listen error");
        close(s_fd);
        exit(1);
    }

    log_info("Server waiting for connection on port: %d...", port);

    while (1)
    {
        len = sizeof(client_addr);

        c_fd = accept(s_fd, (struct sockaddr *)&client_addr, &len);
        if (c_fd == -1)
        {
            log_error("Accept error");
            close(s_fd);
            exit(1);
        }

        log_info("Connection established with client address: %s, port: %d", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        while (1)
        {
            // // 发送数据给客户端
            // const char *message = "Hello from server!";
            // if (send(c_fd, message, strlen(message), 0) == -1)
            // {
            //     log_error("Send error");
            //     close(c_fd);
            //     break; // 断开连接后，退出当前连接处理循环
            // }

            // 清空缓冲区
            memset(buf, 0, sizeof(buf));

            // 接收客户端消息
            bytes_received = recv(c_fd, buf, MAX_LINE, 0);

            // 打印设备号
            unsigned short device_id = buf[0];
            log_info("\nDevice ID: %02X", device_id); // 十六进制格式打印设备号

            printf("Received data (hex): ");
            print_hex(buf, bytes_received);
            // printf("Received data (buf as string): %s \n", buf);

            if (device_id == DEVICE_DI)
            {
                handle_device_DI(buf, bytes_received, &di_data);
            }
            else
            {
                // 打印设备号后的数据
                printf("Data following device ID (string): ");
                size_t data_length = bytes_received - 1;
                buf[1 + data_length] = (data_length < sizeof(buf) - 1) ? '\0' : buf[sizeof(buf) - 1];
                printf("%s\n", (char *)(buf + 1));
            }

            printf("Finished printing hex data. Total length: %zu\n", bytes_received);
            // 暂停一段时间
            usleep(5000); // 5ms
        }
        close(c_fd);
    }

    close(s_fd);
}
