// commands.h

#ifndef COMMANDS_H
#define COMMANDS_H

#include <stddef.h>
#include "server_recv_send.h"

// 将十六进制字符串转换为字节数组
size_t hex_string_to_bytes(const char *hex_str, unsigned char *buffer, size_t buffer_size);

void process_command_file(ThreadData *data);

#endif // COMMANDS_H
