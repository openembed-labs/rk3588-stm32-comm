#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <pthread.h>
#include <ctype.h>
#include <fcntl.h> // For O_CREAT, O_EXCL
#include "logger.h"
#include "di_data.h"
#include "common.h"
#include "server.h"
#include "send_device_data.h"
#include "server_recv_send.h"
#include "send_test_data.h"
#include "commands.h"
#include "heartbeat.h"
#include "gpio_control.h"

void *recv_thread(void *arg);
void *send_thread(void *arg);
void *process_command_file_thread(void *arg);

void server_recv_send(int client_fd)
{

    ThreadData thread_data;
    pthread_t recv_tid, send_tid, command_tid;

    // Initialize thread data
    thread_data.client_fd = client_fd;
    thread_data.is_upgrading = 0;
    pthread_mutex_init(&thread_data.mutex, NULL);

    // Initialize heartbeat mechanism
    heartbeat_init(5); // 初始化心跳检测，超时为5秒

    // Create threads for receiving, sending, and processing commands
    pthread_create(&recv_tid, NULL, recv_thread, &thread_data);
    pthread_create(&send_tid, NULL, send_thread, &thread_data);
    pthread_create(&command_tid, NULL, process_command_file_thread, &thread_data);

    // Wait for threads to finish
    pthread_join(recv_tid, NULL);
    pthread_join(send_tid, NULL);
    pthread_join(command_tid, NULL);

    close(client_fd);
    pthread_mutex_destroy(&thread_data.mutex);
}

void *recv_thread(void *arg)
{
    ThreadData *data = (ThreadData *)arg;
    unsigned char buf[MAX_LINE];
    ssize_t bytes_received;
    DI_Data di_data;

    while (1)
    {
        memset(buf, 0, sizeof(buf));
        bytes_received = recv(data->client_fd, buf, MAX_LINE, 0);

        if (bytes_received <= 0)
        {
            log_error("Client disconnected or error occurred");
            if (heartbeat_is_timeout()) // 检查是否超时
            {
                log_info("Heartbeat timeout. Triggering system reset.");
                reset_stm32();
            }
            break;
        }

        heartbeat_update(); // 每次收到数据后更新心跳

        unsigned short device_id = buf[0];
        log_info("\nDevice ID: %02X", device_id);

        printf("Received data (raw): ");
        fwrite(buf, sizeof(char), bytes_received, stdout);

        printf("\nReceived data (hex): ");
        print_hex(buf, bytes_received);

        // 处理 DI 数据
        if (device_id == DEVICE_DI)
        {
            handle_device_DI(buf, bytes_received, &di_data);
        }
        else
        {
            // 处理其他设备数据
            printf("Data following device ID (string): ");
            size_t data_length = bytes_received - 1;
            buf[1 + data_length] = (data_length < sizeof(buf) - 1) ? '\0' : buf[sizeof(buf) - 1];
            printf("%s\n", (char *)(buf + 1));
        }

        // 准备响应
        // pthread_mutex_lock(&data->mutex);
        // snprintf(data->response, sizeof(data->response), "Message received and processed.");
        // pthread_mutex_unlock(&data->mutex);

        // 准备响应 (避免锁的使用，如果固定消息不依赖共享资源)
        snprintf(data->response, sizeof(data->response), "Message received and processed.");

        usleep(5000); // 5ms
    }

    return NULL;
}

void *send_thread(void *arg)
{
    ThreadData *data = (ThreadData *)arg;
    int retry_count = 0;

    while (1)
    {
        // 如果正在升级，则暂停发送
        if (data->is_upgrading)
        {
            usleep(10000); // 休眠 10ms 后再次检查
            continue;
        }

        pthread_mutex_lock(&data->mutex);

        if (mode_socket == SOCKET_SEND)
        {
            if (send_test_data(data->client_fd) < 0)
            {
                log_error("Send test data failed, retrying...");
                retry_count++;
                if (retry_count >= SEND_RETRY_LIMIT)
                {
                    log_error("Send test data failed after multiple attempts.");
                    break; // 超过重试限制后退出
                }
            }
            else
            {
                retry_count = 0; // 成功发送后重置重试计数
            }
        }

        pthread_mutex_unlock(&data->mutex);
        usleep(10000); // 每 10ms 检查一次
    }

    return NULL;
}

void *process_command_file_thread(void *arg)
{
    ThreadData *data = (ThreadData *)arg;

    // 创建或打开命名信号量
    sem_t *sem = sem_open(SEMAPHORE_NAME, O_CREAT, 0644, 0);
    if (sem == SEM_FAILED)
    {
        log_error("Failed to create or open semaphore in process_command_file_thread.");
        return NULL;
    }

    while (1)
    {
        // 等待信号量的通知
        if (sem_wait(sem) == -1)
        {
            log_error("sem_wait failed");
            break;
        }

        // 锁定互斥量以处理命令文件
        pthread_mutex_lock(&data->mutex);
        process_command_file(data); // 处理命令文件的函数
        pthread_mutex_unlock(&data->mutex);
    }

    // 关闭并解锁信号量
    sem_close(sem);
    sem_unlink(SEMAPHORE_NAME);

    return NULL;
}
