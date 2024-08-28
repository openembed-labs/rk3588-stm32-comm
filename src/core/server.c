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

void run_server(const char *address, int port)
{
    int s_fd, c_fd;
    unsigned char buf[MAX_LINE];
    struct sockaddr_in server_addr, client_addr;
    socklen_t len;
    ssize_t bytes_received;

    s_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (s_fd == -1)
    {
        log_error("Socket creation error");
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

            printf("The bytes_received is: %zu\n", bytes_received);
            printf("Received data (buf as string): %s \n", buf);
            printf("Received data (hex): \n");
            print_hex(buf, bytes_received);

            // 暂停一段时间
            usleep(5000); // 5ms
        }
    }

    close(s_fd);
}
