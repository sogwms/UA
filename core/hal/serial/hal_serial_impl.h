#ifndef __HAL_SERIAL_IMPL_H__
#define __HAL_SERIAL_IMPL_H__

#include <ioal.h>

struct ua_serial_ops;
typedef struct ua_serial
{
    struct ua_device parent;
    const struct ua_serial_ops *ops;

    /** API */
    int (*send)(struct ua_serial *dev, const void *data, uint32_t len);
    int (*recv)(struct ua_serial *dev, void *data, uint32_t len);
}ua_serial_t;
typedef ua_serial_t *ua_serial_p;

typedef struct ua_serial_ops
{
    // int (*configure)(ua_serial_p dev, struct serial_configure *cfg);
    int (*ctrl)(ua_serial_p dev, int cmd, void *arg);
    int (*send)(ua_serial_p dev, const void *data, uint32_t len);
    int (*recv)(ua_serial_p dev, void *data, uint32_t len);
}ua_serial_ops_t;
typedef ua_serial_ops_t *ua_serial_ops_p;

int ua_serial_register(ua_serial_p dev, const char *name, ua_serial_ops_p ops);

int ua_serial_emit_rx_complete_event(ua_serial_p dev, uint32_t length);

#endif // !__HAL_SERIAL_IMPL_H__
