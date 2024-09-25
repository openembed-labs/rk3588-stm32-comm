#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "common.h"
#include "server.h"
#include "logger.h"
#include "socket_utils.h"
#include "server_recv_send.h"
#include "send_test_data_ndev.h"

void setup_server_socket(int *s_fd, const char *address, int port)
{
    struct sockaddr_in server_addr;
    int opt = 1;

    *s_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (*s_fd == -1)
    {
        log_error("Socket creation error");
        exit(1);
    }

    if (setsockopt(*s_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
    {
        log_error("setsockopt error");
        close(*s_fd);
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(address);
    server_addr.sin_port = htons(port);

    log_info("Server address: %s:%d", address, port);

    if (bind(*s_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        log_error("Bind error");
        close(*s_fd);
        exit(1);
    }

    if (listen(*s_fd, MAX_PADDING) == -1)
    {
        log_error("Listen error");
        close(*s_fd);
        exit(1);
    }
}

void accept_connections(int s_fd)
{
    int c_fd;
    struct sockaddr_in client_addr;
    socklen_t len;

    log_info("Server waiting for connection on port: %d...", ntohs(((struct sockaddr_in *)&s_fd)->sin_port));

    while (1)
    {
        len = sizeof(client_addr);
        c_fd = accept(s_fd, (struct sockaddr *)&client_addr, &len);
        if (c_fd == -1)
        {
            log_error("Accept error");
            continue;
        }

        log_info("Connection established with client address: %s, port: %d", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        pid_t pid = fork();
        if (pid < 0)
        {
            log_error("Fork error");
            close(c_fd);
            continue;
        }
        else if (pid == 0)
        {
            close(s_fd); // Close listening socket in child process

            if (mode_socket == SOCKET_ST_NDEV)
            {
                send_test_data_ndev(c_fd);
            }
            else
            {
                server_recv_send(c_fd);
            }

            exit(0);
        }
        else
        {
            close(c_fd); // Close connected socket in parent process
        }
    }
}
