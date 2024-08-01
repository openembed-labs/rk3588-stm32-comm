#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "client.h"
#include "server.h"

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

    if (strcmp(argv[1], "server") == 0)
    {
        run_server(server_address, port);
    }
    else if (strcmp(argv[1], "client") == 0)
    {
        run_client(server_address, port);
    }
    else
    {
        fprintf(stderr, "Invalid mode. Use 'server' or 'client'.\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}
