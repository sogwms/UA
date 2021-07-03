#ifndef __UA_DEF_H__
#define __UA_DEF_H__

#include "./ua_def_ref_compiler.h"
#include "./ua_def_type.h"
#include "./ua_def_init.h"


enum ua_device_type 
{
    UA_DEVICE_CLASS_MISCELLANEOUS = 0,
    UA_DEVICE_CLASS_SPI_BUS,
    UA_DEVICE_CLASS_SPI_DEVICE,
    UA_DEVICE_CLASS_CAN_BUS,

    UA_DEVICE_CLASS2 = 0x00100,

    UA_DEVICE_CLASS3 = 0x10000,
};

enum ua_device_flag 
{
    UA_DEVICE_FLAG_RONLY    = 0x0001,
    UA_DEVICE_FLAG_WONLY    = 0x0002,
    UA_DEVICE_FLAG_RW       = 0x0003,
    UA_DEVICE_FLAG_INT_RX   = 0x0010,
    UA_DEVICE_FLAG_INT_TX   = 0x0020,
};

enum ua_dev_event 
{
    UA_DEV_EVENT_RESERVED = 0,
    UA_DEV_EVENT_RX_COMPLETE,
    UA_DEV_EVENT_TX_COMPLETE,
};

enum ua_status_code
{
    UA_EOK      = 0,
    UA_ERROR    = 1,
    UA_EEMPTY   = 4,
    UA_EIO      = 8,
    UA_EINVAL   = 10,
};

#define UA_ASSERT(exp)

#define UA_ABORT(msg)

#define ua_container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - (unsigned long)(&((type *)0)->member)))


#if 1
#include <stdio.h>
#define ua_printf(...) printf(__VA_ARGS__)

#else
#define ua_printf(...) 
#endif

#endif //
