#include <string.h>
#include "./ioal.h"

struct device_hub_manager
{
    ua_device_p devs[UA_MAX_NUMBER_DEVICE];
    uint32_t dev_counter;
};

static struct device_hub_manager dhm = {0};

int ua_device_register(ua_device_p dev, const char *name, uint32_t type, uint16_t flags)
{
    UA_ASSERT(dev != NULL);

    ua_device_p device;

    if (ua_device_find(name) != NULL) {
        return UA_ERROR;
    }

    strncpy(dev->name, name, UA_MAX_LENGTH_DEV_NAME);
    dev->type = type;
    dev->flag = flags;

#ifdef UA_USING_DEVICE_ID
    dev->id = dhm.dev_counter;
#endif

    dhm.devs[dhm.dev_counter] = dev;
    dhm.dev_counter++;
    ua_printf("UA: registered dev : %s\n", name);
    return UA_EOK;
}

#ifdef UA_USING_DEVICE_ID
int ua_device_find_by_id(uint32_t id, ua_device_p dev)
{
    if (id < dhm.dev_counter) {
        dev = dhm.devs[id];
        return UA_EOK;
    }

    return UA_ERROR;
}
#endif

ua_device_p ua_device_find(const char *name)
{
    ua_device_p device;
    for (uint32_t i=0; i<dhm.dev_counter; i++) {
        device = dhm.devs[i];
        if (device == NULL) {
            UA_ABORT("Faltal error. which shouldn't happen");
        }
        if (strcmp(device->name, name) == 0){
            ua_printf("UA. find device %s\n", name);
            return device;
        }
    }

    return NULL;
}

int ua_device_set_rx_indicate(ua_device_p dev, int (*rx_indicate)(ua_device_p dev, ua_size_t size))
{
    UA_ASSERT(dev != NULL);
    UA_ASSERT(rx_indicate != NULL);

    dev->rx_indicate = rx_indicate;

    return UA_EOK;
}

int ua_device_set_tx_complete(ua_device_p dev, int (*tx_complete)(ua_device_p dev, void *buf))
{
    UA_ASSERT(dev != NULL);
    UA_ASSERT(tx_complete != NULL);

    dev->tx_complete = tx_complete;

    return UA_EOK;
}


int ua_device_init(ua_device_p dev)
{
    UA_ASSERT(dev != NULL);
}

int ua_device_read(ua_device_p dev, ua_off_t pos, void *buf, ua_size_t size)
{
    UA_ASSERT(dev != NULL);
}

int ua_device_write(ua_device_p dev, ua_off_t pos, const void *buf, ua_size_t size)
{
    UA_ASSERT(dev != NULL);
}

int ua_device_ctrl(ua_device_p dev, int cmd, void *args)
{
    UA_ASSERT(dev != NULL);
}