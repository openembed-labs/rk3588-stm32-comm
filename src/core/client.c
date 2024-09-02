#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "client.h"
#include "logger.h"

int c_fd; // 全局变量，用于在线程间共享套接字

// 发送线程函数
void *send_thread_client(void *arg)
{
    char buf[MAX_LINE];
    while (1)
    {
        printf("Enter message to send to server (type 'quit' to exit): ");
        fgets(buf, MAX_LINE, stdin);

        if (strcmp(buf, "quit\n") == 0)
        {
            close(c_fd);
            pthread_exit(NULL);
        }

        int bytes_sent = send(c_fd, buf, strlen(buf), 0);
        if (bytes_sent == -1)
        {
            log_error("Send error");
            close(c_fd);
            pthread_exit(NULL);
        }
        log_info("Sent %d bytes to server", bytes_sent);
    }
}

// 接收线程函数
void *recv_thread_client(void *arg)
{
    char buf[MAX_LINE];
    while (1)
    {
        int len = recv(c_fd, buf, MAX_LINE - 1, 0);
        if (len == -1)
        {
            log_error("Receive error");
            close(c_fd);
            pthread_exit(NULL);
        }
        else if (len == 0)
        {
            log_info("Server closed the connection");
            close(c_fd);
            pthread_exit(NULL);
        }

        buf[len] = '\0';
        printf("Received reply from server: %s\n", buf); // 使用 printf 进行调试
    }
}

void client_main(const char *server_address, int port)
{
    struct sockaddr_in client_addr;

    if ((c_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        log_error("Socket error");
        exit(1);
    }

    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = inet_addr(server_address);
    client_addr.sin_port = htons(port);

    int status = connect(c_fd, (struct sockaddr *)&client_addr, sizeof(client_addr));
    if (status == -1)
    {
        log_error("Connect error");
        close(c_fd);
        exit(1);
    }

    log_info("Connected to server %s on port %d", server_address, port);

    pthread_t send_tid, recv_tid;

    // 创建发送线程
    if (pthread_create(&send_tid, NULL, send_thread_client, NULL) != 0)
    {
        log_error("Failed to create send thread");
        close(c_fd);
        exit(1);
    }

    // 创建接收线程
    if (pthread_create(&recv_tid, NULL, recv_thread_client, NULL) != 0)
    {
        log_error("Failed to create receive thread");
        close(c_fd);
        exit(1);
    }

    // 等待线程结束
    pthread_join(send_tid, NULL);
    pthread_join(recv_tid, NULL);

    close(c_fd);
}
