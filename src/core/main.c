#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include "client.h"
#include "server.h"
#include "daemonize.h"
#include "logger.h"

int main(int argc, char *argv[])
{
    const char *server_address = DEFAULT_ADDRESS;
    int port = DEFAULT_PORT;
    int run_as_daemon = 0;

    if (argc < 2 || argc > 5)
    {
        fprintf(stderr, "Usage: %s <mode> [<server_address> <port>] [--daemon]\n", argv[0]);
        fprintf(stderr, "mode: 'server' or 'client'\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--daemon") == 0)
        {
            run_as_daemon = 1;
        }
        else if (i == 1)
        {
            // 第一参数是模式
            if (strcmp(argv[i], "server") != 0 && strcmp(argv[i], "client") != 0)
            {
                fprintf(stderr, "Invalid mode. Use 'server' or 'client'.\n");
                exit(EXIT_FAILURE);
            }
        }
        else if (i == 2)
        {
            // 第二参数是服务器地址
            server_address = argv[i];
        }
        else if (i == 3)
        {
            // 第三参数是端口号
            port = atoi(argv[i]);
        }
    }

    // 如果选项中包含 --daemon，则将程序变成守护进程
    if (run_as_daemon)
    {
        daemonize();
        set_log_mode(LOG_MODE_SYSLOG);
    }
    else
    {
        set_log_mode(LOG_MODE_CONSOLE);
    }

    // 打开系统日志
    openlog("mydaemon", LOG_PID, LOG_DAEMON);

    if (strcmp(argv[1], "server") == 0)
    {
        log_info("Starting server at %s:%d", server_address, port);
        run_server(server_address, port);
    }
    else if (strcmp(argv[1], "client") == 0)
    {
        log_info("Starting client to connect to %s:%d", server_address, port);
        run_client(server_address, port);
    }
    else
    {
        log_error("Invalid mode. Use 'server' or 'client'.");
        exit(EXIT_FAILURE);
    }

    // 关闭系统日志
    closelog();

    return 0;
}
