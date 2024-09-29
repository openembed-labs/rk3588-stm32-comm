#ifndef SOCKET_UTILS_H
#define SOCKET_UTILS_H

#define MAX_RECONNECT_ATTEMPTS 5
#define RECONNECT_TIMEOUT 5 // seconds

void setup_server_socket(int *s_fd, const char *address, int port);
void accept_connections(int s_fd);

#endif // SOCKET_UTILS_H
