#include <string.h>
#include <ioal.h>

#define LOG_TAG "ua ioal"
#include <elog.h>

struct device_hub_manager
{
    ua_device_p devs[UA_MAX_NUMBER_DEVICE];
    uint32_t dev_counter;
};

static struct device_hub_manager dhm = {0};

int ua_device_register(ua_device_p dev, const char *name, ua_device_ops_p ops, uint32_t type, uint16_t flags)
{
    UA_ASSERT(dev != NULL);
    // UA_ASSERT(ops != NULL);

    if (ua_device_find(name) != NULL) {
        return UA_ERROR;
    }

    strncpy(dev->name, name, UA_MAX_LENGTH_DEV_NAME);
    dev->type = type;
    dev->flag = flags;

#ifdef UA_USING_DEVICE_ID
    dev->id = dhm.dev_counter;
#endif

    dev->_ops = ops;

    dhm.devs[dhm.dev_counter] = dev;
    dhm.dev_counter++;
    log_d("UA: registered dev : %s", name);
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
            log_a("Faltal error. which shouldn't happen");
        }
        if (strcmp(device->name, name) == 0){
            log_d("UA. find device %s", name);
            return device;
        }
    }

    return NULL;
}

int ua_device_set_callback(ua_device_p dev, ua_dev_callback_t cb)
{
    UA_ASSERT(dev != NULL);
    UA_ASSERT(cb != NULL);

    dev->_callback = cb;

    return UA_EOK;
}

int ua_device_event_handler(ua_device_p dev, int event, void *content)
{
    UA_ASSERT(dev != NULL);

    ua_dev_callback_t cb = dev->_callback;
    if (cb != NULL) {
        cb(dev, event, content);
    }

    return UA_EOK;
}

int ua_device_init(ua_device_p dev)
{
    UA_ASSERT(dev != NULL);
    return UA_EOK;
}

int ua_device_read(ua_device_p dev, ua_off_t pos, void *buf, ua_size_t size)
{
    UA_ASSERT(dev != NULL);
    return UA_EOK;
}

int ua_device_write(ua_device_p dev, ua_off_t pos, const void *buf, ua_size_t size)
{
    UA_ASSERT(dev != NULL);
    return UA_EOK;
}

int ua_device_ctrl(ua_device_p dev, int cmd, void *args)
{
    UA_ASSERT(dev != NULL);
    return UA_EOK;
}

int ua_device_check_class(ua_device_p dev, uint32_t class)
{
    UA_ASSERT(dev != NULL);
    if ((dev->type&class) == class) {
        return true;
    }

    return false;
}