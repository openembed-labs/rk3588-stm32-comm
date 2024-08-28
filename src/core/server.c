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

void handle_client(int client_fd);
void setup_server_socket(int *s_fd, const char *address, int port);
void accept_connections(int s_fd);

void server_main(const char *address, int port)
{
    int s_fd;

    setup_server_socket(&s_fd, address, port);
    accept_connections(s_fd);

    close(s_fd);
}

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
            continue; // Continue to accept next connections
        }

        log_info("Connection established with client address: %s, port: %d", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        pid_t pid = fork();
        if (pid < 0)
        {
            log_error("Fork error");
            close(c_fd);
            continue; // Continue to accept next connections
        }
        else if (pid == 0)
        {
            // Child process
            close(s_fd); // Close listening socket in child process
            handle_client(c_fd);
            exit(0);
        }
        else
        {
            // Parent process
            close(c_fd); // Close connected socket in parent process
        }
    }
}

void handle_client(int client_fd)
{
    unsigned char buf[MAX_LINE];
    ssize_t bytes_received;
    DI_Data di_data;

    while (1)
    {
        memset(buf, 0, sizeof(buf));
        bytes_received = recv(client_fd, buf, MAX_LINE, 0);

        if (bytes_received <= 0)
        {
            log_error("Client disconnected or error occurred");
            break;
        }

        unsigned short device_id = buf[0];
        log_info("\nDevice ID: %02X", device_id);

        printf("Received data (hex): ");
        print_hex(buf, bytes_received);

        if (device_id == DEVICE_DI)
        {
            handle_device_DI(buf, bytes_received, &di_data);
        }
        else
        {
            printf("Data following device ID (string): ");
            size_t data_length = bytes_received - 1;
            buf[1 + data_length] = (data_length < sizeof(buf) - 1) ? '\0' : buf[sizeof(buf) - 1];
            printf("%s\n", (char *)(buf + 1));
        }

        printf("Finished printing hex data. Total length: %zu\n", bytes_received);
        usleep(5000); // 5ms
    }

    close(client_fd);
}
