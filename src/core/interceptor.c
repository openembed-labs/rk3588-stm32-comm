#include "interceptor.h"
#include "logger.h"
#include <sys/socket.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

// 定义存储拦截器的数组和计数器
#define MAX_INTERCEPTORS 10

static send_interceptor_fn send_interceptors[MAX_INTERCEPTORS];
static recv_interceptor_fn recv_interceptors[MAX_INTERCEPTORS];
static int send_interceptor_count = 0;
static int recv_interceptor_count = 0;

// 注册 send 拦截器
void register_send_interceptor(send_interceptor_fn interceptor)
{
    if (send_interceptor_count < MAX_INTERCEPTORS)
    {
        send_interceptors[send_interceptor_count++] = interceptor;
    }
    else
    {
        log_error("Send interceptor array is full");
    }
}

// 注册 recv 拦截器
void register_recv_interceptor(recv_interceptor_fn interceptor)
{
    if (recv_interceptor_count < MAX_INTERCEPTORS)
    {
        recv_interceptors[recv_interceptor_count++] = interceptor;
    }
    else
    {
        log_error("Receive interceptor array is full");
    }
}

// 使用拦截器链执行 send 操作
static ssize_t execute_send_interceptors(int socket, const void *buf, size_t len, int flags)
{
    ssize_t result = (ssize_t)len; // 初始化为传入的长度
    for (int i = 0; i < send_interceptor_count; ++i)
    {
        result = send_interceptors[i](socket, buf, len, flags);
        if (result < 0)
        {
            log_error("Send interceptor failed with error: %s", strerror(errno));
            break;
        }
    }
    return result;
}

// 使用拦截器链执行 recv 操作
static ssize_t execute_recv_interceptors(int socket, void *buf, size_t len, int flags)
{
    ssize_t result = (ssize_t)len; // 初始化为传入的长度
    for (int i = 0; i < recv_interceptor_count; ++i)
    {
        result = recv_interceptors[i](socket, buf, len, flags);
        if (result < 0)
        {
            log_error("Receive interceptor failed with error: %s", strerror(errno));
            break;
        }
    }
    return result;
}

// 使用拦截器链执行 send 操作
ssize_t execute_send_with_interceptor(int socket, const void *buf, size_t len, int flags)
{
    return execute_send_interceptors(socket, buf, len, flags);
}

// 使用拦截器链执行 recv 操作
ssize_t execute_recv_with_interceptor(int socket, void *buf, size_t len, int flags)
{
    return execute_recv_interceptors(socket, buf, len, flags);
}
