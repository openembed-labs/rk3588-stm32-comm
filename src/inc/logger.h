#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <syslog.h>

typedef enum
{
    LOG_MODE_CONSOLE,
    LOG_MODE_SYSLOG
} log_mode_t;

void set_log_mode(log_mode_t mode);
void log_info(const char *fmt, ...);
void log_error(const char *fmt, ...);

#endif // LOGGER_H
