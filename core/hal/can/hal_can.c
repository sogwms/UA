#include <string.h>
#include "hal_can.h"

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

int ua_can_emit_rx_complete_event(ua_can_bus_p bus, uint32_t length)
{
    return ua_device_emit_event(&bus->parent, UA_DEVICE_EVENT_RX_COMPLETE, &length);
}

int ua_can_send(ua_can_bus_p bus, ua_can_msg_p msg)
{
    if (bus == NULL) return UA_ERROR;
    if (bus->ops->send == NULL) return UA_ERROR;

    return bus->ops->send(bus, msg);
}

int ua_can_recv(ua_can_bus_p bus, ua_can_msg_p msg)
{
    if (bus == NULL) return UA_ERROR;
    if (bus->ops->recv == NULL) return UA_ERROR;

    return bus->ops->recv(bus, msg);
}

int ua_can_bus_register(ua_can_bus_p bus, const char *name, ua_can_ops_p ops)
{
    UA_ASSERT(bus != UA_NULL);
    UA_ASSERT(ops != UA_NULL);

    memset(bus, 0, sizeof(*bus));

    bus->ops = ops;
    bus->send = ua_can_send;
    bus->recv = ua_can_recv;

    return ua_device_register(&bus->parent, name, &_ops, UA_DEVICE_CLASS_CAN_BUS, UA_DEVICE_FLAG_RW);
}
