#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <pthread.h>
#include <ctype.h>
#include "logger.h"
#include "di_data.h"
#include "common.h"
#include "server.h"
#include "send_device_data.h"
#include "server_recv_send.h"
#include "send_test_data.h"
#include "commands.h"

#define SEND_RETRY_LIMIT 3 // Max number of retries if sending fails

void *recv_thread(void *arg);
void *send_thread(void *arg);

void server_recv_send(int client_fd)
{
    ThreadData thread_data;
    pthread_t recv_tid, send_tid;

    // Initialize thread data
    thread_data.client_fd = client_fd;
    pthread_mutex_init(&thread_data.mutex, NULL);

    // Create threads for receiving and sending
    pthread_create(&recv_tid, NULL, recv_thread, &thread_data);
    pthread_create(&send_tid, NULL, send_thread, &thread_data);

    // Wait for threads to finish
    pthread_join(recv_tid, NULL);
    pthread_join(send_tid, NULL);

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
            break;
        }

        unsigned short device_id = buf[0];
        log_info("\nDevice ID: %02X", device_id);

        printf("Received data (raw): ");
        fwrite(buf, sizeof(char), bytes_received, stdout);

        printf("Received data (hex): ");
        print_hex(buf, bytes_received);

        // Process device DI data
        if (device_id == DEVICE_DI)
        {
            handle_device_DI(buf, bytes_received, &di_data);
        }
        else
        {
            // Process data for other devices
            printf("Data following device ID (string): ");
            size_t data_length = bytes_received - 1;
            buf[1 + data_length] = (data_length < sizeof(buf) - 1) ? '\0' : buf[sizeof(buf) - 1];
            printf("%s\n", (char *)(buf + 1));
        }

        // Prepare the response
        pthread_mutex_lock(&data->mutex);
        snprintf(data->response, sizeof(data->response), "Message received and processed.");
        pthread_mutex_unlock(&data->mutex);

        usleep(5000); // 5ms
    }

    return NULL;
}

void *send_thread(void *arg)
{
    ThreadData *data = (ThreadData *)arg;

    while (1)
    {
        pthread_mutex_lock(&data->mutex);

        process_command_file(data); // TODO 似乎会一直运行

        if (mode_socket == SOCKET_SEND)
        {
            send_test_data(data->client_fd);
        }

        pthread_mutex_unlock(&data->mutex);

        usleep(10000); // 每 10ms 检查一次
    }

    return NULL;
}