#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/socket.h>
#include "common.h"
#include "safe_recv_send.h"

#define BUFFER_SIZE 1024

/**
 * @brief 固件升级  send_firmware(client_sock, "firmware.bin");
 *
 * @param client_sock
 * @param firmware_file "firmware.bin"
 */
void send_firmware(int client_sock, const char *firmware_file)
{
    int file_fd = open(firmware_file, O_RDONLY);
    if (file_fd < 0)
    {
        perror("Failed to open firmware file");
        return;
    }

    char buffer[BUFFER_SIZE];
    int bytes_read;
    while ((bytes_read = read(file_fd, buffer, BUFFER_SIZE)) > 0)
    {
        if (safe_send_interceptor(client_sock, buffer, bytes_read, 0) < 0)
        {
            perror("Failed to send firmware data");
            break;
        }
    }

    close(file_fd);

    // 提示升级完成
    printf("Firmware transfer completed successfully.\n");
}
