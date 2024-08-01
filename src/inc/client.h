#ifndef CLIENT_H
#define CLIENT_H

#define MAX_LINE 256
#define DEFAULT_SERVER_ADDRESS "192.168.88.228"
#define DEFAULT_PORT 5000

void run_client(const char *server_address, int port);

#endif
