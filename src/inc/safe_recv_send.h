#include <stdio.h>

ssize_t safe_send(int socket, const void *buf, size_t len, int flags);
ssize_t safe_recv(int socket, void *buf, size_t len, int flags);