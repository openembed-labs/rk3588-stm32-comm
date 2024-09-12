#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

typedef struct
{
    int client_fd;
    pthread_mutex_t mutex;
    char response[256];
    int is_upgrading; // 新增升级标志
} ThreadData;

void server_recv_send(int client_fd);

#endif // CLIENT_HANDLER_H
