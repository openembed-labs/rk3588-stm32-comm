#ifndef DI_DATA_H
#define DI_DATA_H

#include <stddef.h>

typedef struct
{
    unsigned char DI;
    unsigned char DI_9;
    unsigned char DI_10;
    unsigned char DI_values[8];
} DI_Data;

void generate_di_data(DI_Data *data);
void encode_di_data(const unsigned char *data, unsigned char *buf);
void encode_di_data_struct(const DI_Data *data, unsigned char *buf);
void decode_di_data(const unsigned char *buf, DI_Data *data);
void decode_di_data_struct(const unsigned char *buf, DI_Data *data);
void send_di_data(int connection_fd, const DI_Data *data);
void receive_di_data(int connection_fd, DI_Data *data);
void handle_device_DI(const unsigned char *buf, size_t bytes_received, DI_Data *di_data);

#endif // DI_DATA_H
