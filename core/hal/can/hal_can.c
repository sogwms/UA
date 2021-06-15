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

int ua_can_bus_register(ua_can_bus_p bus, const char *name, ua_can_ops_p ops)
{
    UA_ASSERT(bus != UA_NULL);
    UA_ASSERT(ops != UA_NULL);

    bus->ops = ops;

    ua_device_p device = (ua_device_p)bus;

    device->init = init;
    device->write = write;
    device->read = read;
    device->ctrl = ctrl;

    return ua_device_register(device, name, UA_DEVICE_CLASS_CAN_BUS, UA_DEVICE_FLAG_RW);
}