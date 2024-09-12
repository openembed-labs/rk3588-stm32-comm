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

// 十六进制的开始和结束标识
const unsigned char START_FLAG[] = {0xAA, 0xBB, 0xCC, 0xDD}; // 开始标识
const unsigned char END_FLAG[] = {0xDD, 0xCC, 0xBB, 0xAA};   // 结束标识

/**
 * @brief 发送固件文件（包括开始和结束标识）
 *
 * @param client_sock 客户端套接字
 * @param firmware_file 固件文件路径
 */
void send_firmware(int client_sock, const char *firmware_file)
{
    // 发送开始标识
    if (safe_send(client_sock, START_FLAG, sizeof(START_FLAG), 0) < 0)
    {
        perror("Failed to send start flag");
        return;
    }
    printf("Start flag sent successfully.\n");

    // 打开固件文件
    int file_fd = open(firmware_file, O_RDONLY);
    if (file_fd < 0)
    {
        perror("Failed to open firmware file");
        return;
    }

    char buffer[BUFFER_SIZE];
    int bytes_read;

    // 逐步读取文件并发送
    while ((bytes_read = read(file_fd, buffer, BUFFER_SIZE)) > 0)
    {
        if (safe_send(client_sock, buffer, bytes_read, 0) < 0)
        {
            perror("Failed to send firmware data");
            close(file_fd);
            return;
        }
    }

    close(file_fd);

    // 发送结束标识
    if (safe_send(client_sock, END_FLAG, sizeof(END_FLAG), 0) < 0)
    {
        perror("Failed to send end flag");
        return;
    }
    printf("End flag sent successfully.\n");

    // 提示升级完成
    printf("Firmware transfer completed successfully.\n");
}
