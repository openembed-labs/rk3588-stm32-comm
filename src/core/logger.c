#include "logger.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <syslog.h>

static log_mode_t current_log_mode = LOG_MODE_CONSOLE;

void set_log_mode(log_mode_t mode)
{
    if (current_log_mode != mode)
    {
        if (mode == LOG_MODE_SYSLOG)
        {
            openlog("mydaemon", LOG_PID, LOG_DAEMON);
        }
        else
        {
            closelog();
        }
        current_log_mode = mode;
    }
}

void log_info(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), fmt, args);

    if (current_log_mode == LOG_MODE_CONSOLE)
    {
        printf("%s\n", buffer);
    }
    else
    {
        syslog(LOG_INFO, "%s", buffer);
    }

    va_end(args);
}

void log_error(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer) - 1, ": %s", strerror(errno));

    if (current_log_mode == LOG_MODE_CONSOLE)
    {
        fprintf(stderr, "ERROR: %s\n", buffer);
    }
    else
    {
        syslog(LOG_ERR, "%s", buffer);
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
}

void print_binary(unsigned char byte)
{
    for (int i = 7; i >= 0; i--)
    {
        printf("%d", (byte >> i) & 1);
    }
    printf("\n");
}
