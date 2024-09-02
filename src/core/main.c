#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include "client.h"
#include "server.h"
#include "interactive.h"
#include "daemonize.h"
#include "logger.h"
#include "common.h"

int main(int argc, char *argv[])
{
    const char *server_address = DEFAULT_ADDRESS;
    int port = DEFAULT_PORT;
    int run_as_daemon = 0;

    if (argc < 2 || argc > 6)
    {
        fprintf(stderr, "Usage: %s <mode> [<server_address> <port>] [--daemon] [--send-test | --recv-test]\n", argv[0]);
        fprintf(stderr, "mode: 'server', 'client', or 'interactive'\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--daemon") == 0)
        {
            run_as_daemon = 1;
        }
        else if (strcmp(argv[i], "--send-test") == 0)
        {
            MODE_SEND_TEST = 1;
        }
        else if (strcmp(argv[i], "--recv-test") == 0)
        {
            MODE_RECV_TEST = 1;
        }
        else if (i == 1)
        {
            if (strcmp(argv[i], "server") != 0 && strcmp(argv[i], "client") != 0 && strcmp(argv[i], "interactive") != 0)
            {
                fprintf(stderr, "Invalid mode. Use 'server', 'client', or 'interactive'.\n");
                exit(EXIT_FAILURE);
            }
        }
        else if (i == 2)
        {
            server_address = argv[i];
        }
        else if (i == 3)
        {
            port = atoi(argv[i]);
        }
    }

    if (run_as_daemon)
    {
        daemonize();
        set_log_mode(LOG_MODE_SYSLOG);
    }
    else
    {
        set_log_mode(LOG_MODE_CONSOLE);
    }

    openlog("mydaemon", LOG_PID, LOG_DAEMON);

    if (strcmp(argv[1], "server") == 0)
    {
        log_info("Starting server at %s:%d", server_address, port);
        server_main(server_address, port);
    }
    else if (strcmp(argv[1], "client") == 0)
    {
        log_info("Starting client to connect to %s:%d", server_address, port);
        client_main(server_address, port);
    }
    else if (strcmp(argv[1], "interactive") == 0)
    {
        log_info("Entering interactive mode (enter '/exit' to exit): .");
        interactive_mode_main(COMMAND_FILE);
    }
    else
    {
        log_error("Invalid mode. Use 'server', 'client', or 'interactive'.");
        exit(EXIT_FAILURE);
    }

    closelog();
    return 0;
}
