#include "logger.h"
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>

ssize_t safe_send(int socket, const void *buf, size_t len, int flags)
{

    ssize_t bytes_sent = send(socket, buf, len, flags);
    if (bytes_sent < 0)
    {
        log_error("Failed to send data");
    }
    usleep(50000); // Delay after sending data

    return bytes_sent;
}

ssize_t safe_recv(int socket, void *buf, size_t len, int flags)
{

    ssize_t bytes_received = recv(socket, buf, len, flags);
    if (bytes_received < 0)
    {
        log_error("Failed to receive data");
    }
    else if (bytes_received == 0)
    {
        log_info("Connection closed by peer");
    }

    return bytes_received;
}
