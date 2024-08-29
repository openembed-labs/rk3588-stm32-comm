#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

void server_recv_send(int client_fd);
int send_to_client(int client_fd, const char *message);

#endif // CLIENT_HANDLER_H
