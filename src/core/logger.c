#include "logger.h"
#include <stdarg.h>

static log_mode_t current_log_mode = LOG_MODE_CONSOLE;

void set_log_mode(log_mode_t mode)
{
    current_log_mode = mode;
    if (mode == LOG_MODE_SYSLOG)
    {
        openlog("mydaemon", LOG_PID, LOG_DAEMON);
    }
    else
    {
        closelog();
    }
}

void log_info(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    if (current_log_mode == LOG_MODE_CONSOLE)
    {
        vprintf(fmt, args);
        printf("\n");
    }
    else
    {
        vsyslog(LOG_INFO, fmt, args);
    }

    va_end(args);
}

void log_error(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    if (current_log_mode == LOG_MODE_CONSOLE)
    {
        vfprintf(stderr, fmt, args);
        fprintf(stderr, "\n");
    }
    else
    {
        vsyslog(LOG_ERR, fmt, args);
    }

    va_end(args);
}

void print_hex(const unsigned char *buf, size_t length)
{

    for (size_t i = 0; i < length; i++)
    {
        printf("%02X ", buf[i]);
    }

    printf("\n");

    printf("Finished printing hex data. Total length: %zu\n", length);
}
