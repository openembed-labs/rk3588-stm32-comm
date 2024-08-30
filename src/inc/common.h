#ifndef COMMON_H
#define COMMON_H

#define MODE_TEST 1 // 测试模式

#define DEVICE_RS485_1 0x01
#define DEVICE_RS485_2 0x02
#define DEVICE_RS485_3 0x03
#define DEVICE_RS485_4 0x04

#define DEVICE_RS232_1 0x05
#define DEVICE_RS232_2 0x06

#define DEVICE_CAN_1 0x07
#define DEVICE_CAN_2 0x08

#define DEVICE_DI 0x09
#define DEVICE_DO 0x0A

// #define DELAY_MICROSECOND 10000 // 10ms delay after each send
#define DELAY_MICROSECOND 0 // 0ms delay after each send

#endif
