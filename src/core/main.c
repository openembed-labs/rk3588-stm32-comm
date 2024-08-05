#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include "client.h"
#include "server.h"
#include "daemonize.h"

int main(int argc, char *argv[])
{
    const char *server_address = DEFAULT_SERVER_ADDRESS;
    int port = DEFAULT_PORT;

    if (argc < 2 || argc > 4)
    {
        fprintf(stderr, "Usage: %s <mode> [<server_address> <port>]\n", argv[0]);
        fprintf(stderr, "mode: 'server' or 'client'\n");
        exit(EXIT_FAILURE);
    }

    if (argc >= 3)
    {
        server_address = argv[2];
    }
    if (argc == 4)
    {
        port = atoi(argv[3]);
    }

    // 使程序成为守护进程
    daemonize();

    if (strcmp(argv[1], "server") == 0)
    {
        syslog(LOG_INFO, "Starting server at %s:%d", server_address, port);
        run_server(server_address, port);
    }
    else if (strcmp(argv[1], "client") == 0)
    {
        syslog(LOG_INFO, "Starting client to connect to %s:%d", server_address, port);
        run_client(server_address, port);
    }
    else
    {
        syslog(LOG_ERR, "Invalid mode. Use 'server' or 'client'.");
        exit(EXIT_FAILURE);
    }

    // 关闭系统日志
    closelog();

    return 0;
}
