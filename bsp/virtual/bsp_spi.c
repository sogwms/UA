#include <hal_spi_impl.h>

static int configure(struct ua_spi_device *device, struct ua_spi_configuration *configuration)
{
    return UA_EOK;
}

static int transfer(struct ua_spi_device *device, struct ua_spi_message *message)
{
    void *cs_obj = device->parent.user_data;

    return UA_EOK;
}

static struct ua_spi_ops ops = {
    .configure = configure,
    .transfer = transfer,
};

static ua_spi_bus_t bus;

int bsp_spi_init(void)
{
    return ua_spi_bus_register(&bus, "spi1", &ops);
}

// UA_EXPORT_INIT_BOARD(bsp_spi_init);
