#ifndef HEARTBEAT_H
#define HEARTBEAT_H

#include <time.h>

// 初始化心跳检测，传入超时时间
void heartbeat_init(time_t timeout_interval);

// 更新心跳，记录最近一次接收数据的时间
void heartbeat_update();

// 停止心跳监控（如果需要手动停止时调用）
void heartbeat_stop();

// 检查心跳是否超时
int heartbeat_is_timeout();

#endif // HEARTBEAT_H
