#ifndef __HAL_CAN_H__
#define __HAL_CAN_H__

#include <ioal.h>

typedef struct ua_can_ops
{
    
}ua_can_ops_t;

typedef struct ua_can_bus
{
    struct ua_device parent;
    const ua_can_ops_t *ops;
}ua_can_bus_t;
typedef ua_can_bus_t *ua_can_bus_p;

#define UA_CAN_ID_TYPE_STD          (0)
#define UA_CAN_ID_TYPE_EXT          (1)

#define UA_CAN_FRAME_TYPE_DATA      (0)
#define UA_CAN_FRAME_TYPE_REMOTE    (1)

typedef struct ua_can_msg
{
    uint32_t id         : 29;
    uint32_t id_type    : 1;        // standard or extended id  
    uint32_t frame_type : 1;        // data or remote frame  
    uint32_t : 1;
    uint8_t data_length;
    uint8_t data[8];
}ua_can_msg_t;
typedef ua_can_msg_t *ua_can_msg_p;

int ua_can_send(ua_can_bus_p dev, ua_can_msg_p msg);
