#ifndef __IOAL_H__
#define __IOAL_H__

#include <ua_def.h>
#include "../ua_cfg.h"

typedef struct ua_device *ua_device_p;

typedef int (*ua_dev_callback_t)(ua_device_p dev, int event, void *content);

typedef struct ua_device
{
    char name[UA_MAX_LENGTH_DEV_NAME];
    uint32_t type;
    uint16_t flag;

#ifdef UA_USING_DEVICE_ID
    uint32_t id;
#endif

    int (*init)(ua_device_p dev);
    int (*read)(ua_device_p dev, ua_off_t pos, void *buf, ua_size_t size);
    int (*write)(ua_device_p dev, ua_off_t pos, const void *buf, ua_size_t size);
    int (*ctrl)(ua_device_p dev, int cmd, void *args);

    ua_dev_callback_t _callback;

    void *user_data;
}ua_device_t;

int ua_device_register(ua_device_p dev, const char *name, uint32_t type, uint16_t flags);

ua_device_p ua_device_find(const char *name);

int ua_device_set_callback(ua_device_p dev, ua_dev_callback_t cb);

int ua_device_init(ua_device_p dev);
int ua_device_read(ua_device_p dev, ua_off_t pos, void *buf, ua_size_t size);
int ua_device_write(ua_device_p dev, ua_off_t pos, const void *buf, ua_size_t size);
int ua_device_ctrl(ua_device_p dev, int cmd, void *args);

#ifdef UA_USING_DEVICE_ID
int ua_device_find_by_id(uint32_t id, ua_device_p dev);
#endif

#endif //__IOAL_H__
