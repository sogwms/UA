#ifndef __HAL_SERIAL_H__
#define __HAL_SERIAL_H__

#include "hal_serial_impl.h"

int ua_serial_send(ua_serial_p dev, const void *data, uint32_t len);
int ua_serial_recv(ua_serial_p dev, void *data, uint32_t len);

#endif