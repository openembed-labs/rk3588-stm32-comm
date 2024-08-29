#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "server.h"
#include "logger.h"
#include "common.h"
#include "socket_utils.h"

void server_main(const char *address, int port)
{
    int s_fd;
    setup_server_socket(&s_fd, address, port);
    accept_connections(s_fd);
    close(s_fd);
}
