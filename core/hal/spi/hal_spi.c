#include "hal_spi.h"

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

/*** Implementation of spi specific operations ***/
/* send data then receive data from SPI device */
UA_WEAK int ua_spi_send_then_recv(ua_spi_device_p device,
                          const void    *send_buf,
                          ua_size_t      send_len,
                          void          *recv_buf,
                          ua_size_t      recv_len)
{
    int rvl;
    ua_spi_message_t msg;

    UA_ASSERT(device != NULL);
    UA_ASSERT(device->bus != NULL);

    msg.send_buf   = send_buf;
    msg.recv_buf   = NULL;
    msg.length     = send_len;
    msg.cs_take    = 1;
    msg.cs_release = 0;
    rvl = device->bus->ops->transfer(device, &msg);
    if (rvl != UA_EOK) {
        return UA_EIO;
    }

    msg.send_buf   = NULL;
    msg.recv_buf   = recv_buf;
    msg.length     = recv_len;
    msg.cs_take    = 0;
    msg.cs_release = 1;
    rvl = device->bus->ops->transfer(device, &msg);
    if (rvl != UA_EOK) {
        return UA_EIO;
    }

    return UA_EOK;
}

UA_WEAK int ua_spi_send_then_send(ua_spi_device_p device,
                          const void    *buf1,
                          ua_size_t      len1,
                          const void    *buf2,
                          ua_size_t      len2)
{
    int rvl;
    ua_spi_message_t msg;

    UA_ASSERT(device != NULL);
    UA_ASSERT(device->bus != NULL);

    msg.send_buf   = buf1;
    msg.recv_buf   = NULL;
    msg.length     = len1;
    msg.cs_take    = 1;
    msg.cs_release = 0;
    rvl = device->bus->ops->transfer(device, &msg);
    if (rvl != UA_EOK) {
        return UA_EIO;
    }

    msg.send_buf   = buf2;
    msg.recv_buf   = NULL;
    msg.length     = len2;
    msg.cs_take    = 0;
    msg.cs_release = 1;
    rvl = device->bus->ops->transfer(device, &msg);
    if (rvl != UA_EOK) {
        return UA_EIO;
    }

    return UA_EOK;
}

UA_WEAK ua_size_t ua_spi_transfer(ua_spi_device_p device,
                          const void    *send_buf,
                          void          *recv_buf,
                          ua_size_t      len)
{
    int rvl;
    ua_spi_message_t msg;

    UA_ASSERT(device != NULL);
    UA_ASSERT(device->bus != NULL);

    msg.send_buf   = send_buf;
    msg.recv_buf   = recv_buf;
    msg.length     = len;
    msg.cs_take    = 1;
    msg.cs_release = 1;
    rvl = device->bus->ops->transfer(device, &msg);
    if (rvl != UA_EOK) {
        return UA_EIO;
    }

    return UA_EOK;
}

UA_WEAK int ua_spi_configure(ua_spi_device_p device, ua_spi_configuration_p cfg)
{
    UA_ASSERT(device != NULL);

    device->config.data_width = cfg->data_width;
    device->config.max_hz = cfg->max_hz;
    device->config.mode = cfg->mode & UA_SPI_MODE_MASK;

    return device->bus->ops->configure(device, &device->config);
}

int ua_spi_bus_register(ua_spi_bus_p bus, const char *name, ua_spi_ops_p ops)
{
    UA_ASSERT(bus != NULL);
    UA_ASSERT(ops != NULL);

    bus->ops = ops;

    return ua_device_register(&bus->parent, name, &_ops, UA_DEVICE_CLASS_SPI_BUS, UA_DEVICE_FLAG_RW);
}

int ua_spi_bus_attach_device(ua_spi_device_p device, 
                             const char *name,
                             const char *bus_name,
                             void *user_data)
{
    UA_ASSERT (device != NULL);

    ua_device_p bus = NULL;
    bus = (ua_device_p)ua_device_find(bus_name);

    if ((bus != NULL) && (bus->type == UA_DEVICE_CLASS_SPI_BUS)) 
    {
        device->bus = (ua_spi_bus_p)bus;
        device->parent.user_data = user_data;

        device->send_then_send = ua_spi_send_then_send;
        device->send_then_recv = ua_spi_send_then_recv;
        device->transfer = ua_spi_transfer;

        return ua_device_register(&device->parent, name, &_ops, UA_DEVICE_CLASS_SPI_DEVICE, UA_DEVICE_FLAG_RW);
    }

    return UA_ERROR;
}
