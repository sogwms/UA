#ifndef __IOAL_H__
#define __IOAL_H__

#include <ua_def.h>
#include "../ua_cfg.h"

typedef struct ua_device *ua_device_p;

typedef int (*ua_dev_callback_t)(ua_device_p dev, int event, void *content);

struct ua_device_ops;
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
    struct ua_device_ops *_ops;

    void *user_data;
}ua_device_t;

typedef struct ua_device_ops
{
    int (*init)(ua_device_p dev);
    int (*read)(ua_device_p dev, ua_off_t pos, void *buf, ua_size_t size);
    int (*write)(ua_device_p dev, ua_off_t pos, const void *buf, ua_size_t size);
    int (*ctrl)(ua_device_p dev, int cmd, void *args);
}ua_device_ops_t;
typedef ua_device_ops_t * ua_device_ops_p;

ua_device_p ua_device_find(const char *name);
#ifdef UA_USING_DEVICE_ID
int ua_device_find_by_id(uint32_t id, ua_device_p dev);
#endif
int ua_device_register(ua_device_p dev, const char *name, ua_device_ops_p ops, uint32_t type, uint16_t flags);
int ua_device_set_callback(ua_device_p dev, ua_dev_callback_t cb);

int ua_device_init(ua_device_p dev);
int ua_device_read(ua_device_p dev, ua_off_t pos, void *buf, ua_size_t size);
int ua_device_write(ua_device_p dev, ua_off_t pos, const void *buf, ua_size_t size);
int ua_device_ctrl(ua_device_p dev, int cmd, void *args);

int ua_device_event_handler(ua_device_p dev, int event, void *content);

#endif //__IOAL_H__
