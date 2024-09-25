#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include "common.h"
#include "heartbeat.h"
#include "gpio_control.h" // 引入GPIO控制的头文件

volatile time_t last_received_time = 0; // 记录最近一次接收到数据的时间
time_t timeout_interval = 5;            // 默认超时为5秒

/**
 * @brief 超时处理函数
 *
 * @param signum
 */
void timeout_handler(int signum)
{
    time_t current_time = time(NULL);

    // 检查时间差，判断是否超时
    if (difftime(current_time, last_received_time) > timeout_interval)
    {
        printf("STM32H750 might have crashed. Resetting...\n");
        reset_stm32(); // 调用复位函数
    }
    else
    {
        // 如果未超时，则继续启动定时器
        alarm(timeout_interval);
    }
}

/**
 * @brief 初始化心跳检测，设置超时时间
 *
 * @param timeout
 */
void heartbeat_init(time_t timeout)
{
    timeout_interval = timeout;
    last_received_time = time(NULL); // 初始化为当前时间

    // 设置定时器
    signal(SIGALRM, timeout_handler);
    alarm(timeout_interval); // 每隔timeout_interval秒触发一次超时检测
}

/**
 * @brief 更新心跳，表示收到新的数据
 *
 */
void heartbeat_update()
{
    last_received_time = time(NULL); // 更新最近一次接收数据的时间
    // printf("Heartbeat updated.\n");
}

/**
 * @brief 停止心跳监控
 *
 */
void heartbeat_stop()
{
    alarm(0); // 停止定时器
}

/**
 * @brief 检查心跳是否超时
 *
 * @return int
 */
int heartbeat_is_timeout()
{
    time_t current_time = time(NULL);
    return difftime(current_time, last_received_time) > timeout_interval;
}
