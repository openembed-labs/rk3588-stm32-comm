#include "common.h"

Mode mode = MODE_INVALID;
SocketMode mode_socket = SOCKET_INVALID;

const int device_do_ids[] = {
    DEVICE_DO_1, DEVICE_DO_2, DEVICE_DO_3, DEVICE_DO_4, DEVICE_DO_5,
    DEVICE_DO_6, DEVICE_DO_7, DEVICE_DO_8, DEVICE_DO_9, DEVICE_DO_10};

const int device_di_ids[] = {
    DEVICE_DI_1, DEVICE_DI_2, DEVICE_DI_3, DEVICE_DI_4, DEVICE_DI_5,
    DEVICE_DI_6, DEVICE_DI_7, DEVICE_DI_8, DEVICE_DI_9, DEVICE_DI_10};