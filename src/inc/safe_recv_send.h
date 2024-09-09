#include <stdio.h>

ssize_t safe_send_interceptor(int socket, const void *buf, size_t len, int flags);
ssize_t safe_recv_interceptor(int socket, void *buf, size_t len, int flags);