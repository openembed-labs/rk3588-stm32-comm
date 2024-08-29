#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include "logger.h"
#include "di_data.h"
#include "common.h"
#include "server.h"
#include "client_management.h"

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
