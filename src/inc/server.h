#ifndef SERVER_H
#define SERVER_H

#define MAX_PADDING 10
#define MAX_LINE 256
#define DEFAULT_ADDRESS "0.0.0.0" // Default to bind all network interfaces
#define DEFAULT_PORT 5000         // Default port number

void server_main(const char *address, int port);

#endif
