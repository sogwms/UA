#ifndef __HAL_CAN_H__
#define __HAL_CAN_H__

#include "hal_can_impl.h"

int ua_can_send(ua_can_bus_p bus, ua_can_msg_p msg);
int ua_can_recv(ua_can_bus_p bus, ua_can_msg_p msg);

#endif

