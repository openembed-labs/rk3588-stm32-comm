#ifndef SEND_DEVICE_DATA_H
#define SEND_DEVICE_DATA_H

#include <unistd.h>

int send_device_data(int client_fd, unsigned char device_id, const unsigned char *data, size_t data_length);
int send_device_data_pure(int client_fd, unsigned char device_id, const unsigned char *data, size_t data_length);
void construct_and_send_data(int client_fd);

#endif // SEND_DEVICE_DATA_H
