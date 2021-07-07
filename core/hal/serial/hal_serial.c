#include <string.h>
#include "hal_serial.h"

/*** Implementation of general device operations abstraction ***/
static int init(ua_device_p dev)
{
    return UA_EOK;
}
static int read(ua_device_p dev, ua_off_t pos, void *buf, ua_size_t size)
{
    return UA_EOK;
}
static int write(ua_device_p dev, ua_off_t pos, const void *buf, ua_size_t size)
{
    return UA_EOK;
}
static int ctrl(ua_device_p dev, int cmd, void *args)
{
    return UA_EOK;
}

static ua_device_ops_t _ops = {
    .init = init,
    .read = read,
    .write = write,
    .ctrl = ctrl,
};

// called by low-level driver after data is received
int ua_serial_rx_complete(ua_serial_p dev, uint32_t length)
{
    return ua_device_event_handler(&dev->parent, UA_DEVICE_EVENT_RX_COMPLETE, &length);
}

int ua_serial_send(ua_serial_p dev, const void *data, uint32_t len)
{
    if (dev == NULL) return UA_ERROR;
    if (dev->ops->send == NULL) return UA_ERROR;

    return dev->ops->send(dev, data, len);
}

int ua_serial_recv(ua_serial_p dev, void *data, uint32_t len)
{
    if (dev == NULL) return UA_ERROR;
    if (dev->ops->recv == NULL) return UA_ERROR;

    return dev->ops->recv(dev, data, len);
}

int ua_serial_register(ua_serial_p dev, const char *name, ua_serial_ops_p ops)
{
    UA_ASSERT(dev != NULL);
    UA_ASSERT(ops != NULL);

    memset(dev, 0, sizeof(*dev));

    dev->ops = ops;
    dev->send = ua_serial_send;
    dev->recv = ua_serial_recv;

    return ua_device_register(&dev->parent, name, &_ops, UA_DEVICE_CLASS_SERIAL, UA_DEVICE_FLAG_RW);
}

