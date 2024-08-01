#include <stdio.h>
#include <stdlib.h>
#include "client.h"

int main(int argc, char *argv[])
{
    const char *server_address = DEFAULT_SERVER_ADDRESS;
    int port = DEFAULT_PORT;

    if (argc == 3)
    {
        server_address = argv[1];
        port = atoi(argv[2]);
    }
    else if (argc != 1)
    {
        fprintf(stderr, "Usage: %s [<server_address> <port>]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    run_client(server_address, port);

    return 0;
}
