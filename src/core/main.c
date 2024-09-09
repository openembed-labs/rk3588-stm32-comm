#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <syslog.h>
#include "logger.h"
#include "interactive.h"
#include "server.h"
#include "client.h"
#include "daemonize.h"
#include "common.h"
#include "gpio_control.h"
#include "interceptor.h"
#include "safe_recv_send.h"

void parse_arguments(int argc, char *argv[], int *run_as_daemon, Mode *mode, char **server_address, int *port);
void initialize_program(int run_as_daemon);
void start_mode(Mode mode, const char *server_address, int port);
void cleanup(int signum);

static const char *GPIO_PIN_NUMBER = GPIO_PIN; // GPIO 引脚编号

int main(int argc, char *argv[])
{
    int run_as_daemon = 0;
    char *server_address = DEFAULT_ADDRESS;
    int port = DEFAULT_PORT;

    if (argc < 2 || argc > 6)
    {
        fprintf(stderr, "Usage: %s <mode> [<server_address> <port>] [--daemon] [--send-test | --recv-test]\n", argv[0]);
        fprintf(stderr, "mode: 'server', 'client', or 'interactive'\n");
        exit(EXIT_FAILURE);
    }

    parse_arguments(argc, argv, &run_as_daemon, &mode, &server_address, &port);
    initialize_program(run_as_daemon);

    // 初始化 GPIO
    gpio_init_high(GPIO_PIN_NUMBER);

    // 注册信号处理函数
    signal(SIGINT, cleanup);
    signal(SIGTERM, cleanup);

    start_mode(mode, server_address, port);

    // 注册拦截器
    register_send_interceptor(safe_send_interceptor);
    register_recv_interceptor(safe_recv_interceptor);

    // 正常退出
    gpio_cleanup(GPIO_PIN_NUMBER); // 清理 GPIO 电平和资源

    return 0;
}

void parse_arguments(int argc, char *argv[], int *run_as_daemon, Mode *mode, char **server_address, int *port)
{
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--daemon") == 0)
        {
            *run_as_daemon = 1;
        }
        else if (strcmp(argv[i], "--send-test") == 0)
        {
            mode_socket = SOCKET_SEND;
        }
        else if (strcmp(argv[i], "--recv-test") == 0)
        {
            mode_socket = SOCKET_RECV;
        }
        else if (i == 1)
        {
            if (strcmp(argv[i], "server") == 0)
                *mode = MODE_SERVER;
            else if (strcmp(argv[i], "client") == 0)
                *mode = MODE_CLIENT;
            else if (strcmp(argv[i], "interactive") == 0)
                *mode = MODE_INTERACTIVE;
            else
            {
                fprintf(stderr, "Invalid mode. Use 'server', 'client', or 'interactive'.\n");
                exit(EXIT_FAILURE);
            }
        }
        else if (i == 2)
        {
            *server_address = argv[i];
        }
        else if (i == 3)
        {
            *port = atoi(argv[i]);
        }
    }
}

void initialize_program(int run_as_daemon)
{
    if (run_as_daemon)
    {
        daemonize();
        set_log_mode(LOG_MODE_SYSLOG);
    }
    else
    {
        set_log_mode(LOG_MODE_CONSOLE);
    }

    // 检查用户权限
    if (geteuid() != 0)
    {
        log_error("Program requires root privileges to access GPIO. Please run with sudo.");
        exit(EXIT_FAILURE); // 错误兼容处理：退出程序并提示
    }

    openlog("mydaemon", LOG_PID, LOG_DAEMON);
}

void start_mode(Mode mode, const char *server_address, int port)
{
    switch (mode)
    {
    case MODE_SERVER:
        log_info("Starting server at %s:%d", server_address, port);
        server_main(server_address, port);
        break;

    case MODE_CLIENT:
        log_info("Starting client to connect to %s:%d", server_address, port);
        client_main(server_address, port);
        break;

    case MODE_INTERACTIVE:
        log_info("Entering interactive mode.");
        // 打印使用规则
        printf("Usage: Command file should contain lines with the following format:\n");
        printf("       <device_id> <hex_command_data>\n");
        printf("       Where <device_id> is a hexadecimal number (e.g., 0x01) and <hex_command_data> is a hex string (e.g., '01 23 45')\n");
        printf("enter '/exit' to exit\n");
        interactive_mode_main();
        break;

    default:
        log_error("Invalid mode. Use 'server', 'client', or 'interactive'.");
        exit(EXIT_FAILURE);
    }

    sem_unlink(SEMAPHORE_NAME);
    closelog();
}

void cleanup(int signum)
{
    log_info("Received signal %d, cleaning up GPIO and exiting...", signum);

    // 清理 GPIO
    gpio_cleanup(GPIO_PIN_NUMBER);

    closelog();
    exit(EXIT_SUCCESS);
}