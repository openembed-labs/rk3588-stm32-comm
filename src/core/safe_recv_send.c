#include "logger.h"
#include <sys/socket.h>
#include <stdio.h>

ssize_t safe_send(int socket, const void *buf, size_t len, int flags)
{
    // 在发送之前做一些处理
    printf("Sending data through safe_send\n");

    ssize_t bytes_sent = send(socket, buf, len, flags);
    if (bytes_sent < 0)
    {
        log_error("Failed to send data");
    }
    else
    {
        log_info("Sent %zd bytes", bytes_sent);
        print_hex((const unsigned char *)buf, bytes_sent);
    }
    return bytes_sent;
}

ssize_t safe_recv(int socket, void *buf, size_t len, int flags)
{
    // 在接收之前做一些处理
    printf("Receiving data through safe_recv\n");

    ssize_t bytes_received = recv(socket, buf, len, flags);
    if (bytes_received < 0)
    {
        log_error("Failed to receive data");
    }
    else if (bytes_received == 0)
    {
        log_info("Connection closed by peer");
    }
    else
    {
        log_info("Received %zd bytes", bytes_received);
        print_hex((const unsigned char *)buf, bytes_received);
    }
    return bytes_received;
}
