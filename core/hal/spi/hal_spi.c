#include "hal_spi.h"

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

    UA_ASSERT(device != UA_NULL);
    UA_ASSERT(device->bus != UA_NULL);

    msg.send_buf   = send_buf;
    msg.recv_buf   = UA_NULL;
    msg.length     = send_len;
    msg.cs_take    = 1;
    msg.cs_release = 0;
    rvl = device->bus->ops->transfer(device, &msg);
    if (rvl != UA_EOK) {
        return UA_EIO;
    }

    msg.send_buf   = UA_NULL;
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

    UA_ASSERT(device != UA_NULL);
    UA_ASSERT(device->bus != UA_NULL);

    msg.send_buf   = buf1;
    msg.recv_buf   = UA_NULL;
    msg.length     = len1;
    msg.cs_take    = 1;
    msg.cs_release = 0;
    rvl = device->bus->ops->transfer(device, &msg);
    if (rvl != UA_EOK) {
        return UA_EIO;
    }

    msg.send_buf   = buf2;
    msg.recv_buf   = UA_NULL;
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

    UA_ASSERT(device != UA_NULL);
    UA_ASSERT(device->bus != UA_NULL);

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
    UA_ASSERT(device != UA_NULL);

    device->config.data_width = cfg->data_width;
    device->config.max_hz = cfg->max_hz;
    device->config.mode = cfg->mode & UA_SPI_MODE_MASK;

    return device->bus->ops->configure(device, &device->config);
}

/*** Implementation of general device operations abstraction ***/
static int __device_spi_device_read(ua_device_p dev, ua_off_t pos, void *buf, ua_size_t size)
{
    return ua_spi_transfer((ua_spi_device_p)dev, UA_NULL, buf, size);
}
static int __device_spi_device_write(ua_device_p dev, ua_off_t pos, const void *buf, ua_size_t size)
{
    return ua_spi_transfer((ua_spi_device_p)dev, buf, UA_NULL, size);
}
static int __device_spi_device_ctrl(ua_device_p dev, int cmd, void *args)
{
    // TODO
    return UA_EOK;
}

int ua_spi_bus_register(ua_spi_bus_p bus, const char *name, ua_spi_ops_p ops)
{
    UA_ASSERT(bus != UA_NULL);
    UA_ASSERT(ops != UA_NULL);

    bus->ops = ops;

    ua_device_p device = (ua_device_p)bus;

    device->init = UA_NULL;
    device->write = __device_spi_device_write;
    device->read = __device_spi_device_read;
    device->ctrl = __device_spi_device_ctrl;

    return ua_device_register(device, name, UA_DEVICE_CLASS_SPI_BUS, UA_DEVICE_FLAG_RW);
}

int ua_spi_bus_attach_device(ua_spi_device_p device, 
                             const char *name,
                             const char *bus_name,
                             void *user_data)
{
    UA_ASSERT (device != UA_NULL);

    ua_device_p bus = UA_NULL;
    bus = (ua_device_p)ua_device_find(bus_name);

    if ((bus != UA_NULL) && (bus->type == UA_DEVICE_CLASS_SPI_BUS)) 
    {
        device->bus = (ua_spi_bus_p)bus;
        device->parent.user_data = user_data;

        device->parent.init = UA_NULL;
        device->parent.write = __device_spi_device_write;
        device->parent.read = __device_spi_device_read;
        device->parent.ctrl = __device_spi_device_ctrl;

        device->send_then_send = ua_spi_send_then_send;
        device->send_then_recv = ua_spi_send_then_recv;
        device->transfer = ua_spi_transfer;

        return ua_device_register((ua_device_p)device, name, UA_DEVICE_CLASS_SPI_DEVICE, UA_DEVICE_FLAG_RW);
    }

    return UA_ERROR;
}
