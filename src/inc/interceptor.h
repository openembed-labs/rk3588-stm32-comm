#ifndef INTERCEPTOR_H
#define INTERCEPTOR_H

#include <sys/types.h> // For ssize_t

// 定义 send 拦截器函数指针类型
typedef ssize_t (*send_interceptor_fn)(int, const void *, size_t, int);

// 定义 recv 拦截器函数指针类型
typedef ssize_t (*recv_interceptor_fn)(int, void *, size_t, int);

// 注册 send 拦截器
void register_send_interceptor(send_interceptor_fn interceptor);

// 注册 recv 拦截器
void register_recv_interceptor(recv_interceptor_fn interceptor);

// 使用拦截器链执行 send 操作
ssize_t execute_send_with_interceptor(int socket, const void *buf, size_t len, int flags);

// 使用拦截器链执行 recv 操作
ssize_t execute_recv_with_interceptor(int socket, void *buf, size_t len, int flags);

#endif // INTERCEPTOR_H
