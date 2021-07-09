#ifndef __HAL_CAN_IMPL_H__
#define __HAL_CAN_IMPL_H__

#include <ioal.h>

// #define UA_CAN_BAUDRATE_5KB       (    5 * 1000 )
// #define UA_CAN_BAUDRATE_10KB      (   10 * 1000 )
// #define UA_CAN_BAUDRATE_20KB      (   20 * 1000 )
// #define UA_CAN_BAUDRATE_100KB     (  100 * 1000 )
// #define UA_CAN_BAUDRATE_125KB     (  125 * 1000 )
// #define UA_CAN_BAUDRATE_250KB     (  250 * 1000 )   
// #define UA_CAN_BAUDRATE_500KB     (  500 * 1000 )
// #define UA_CAN_BAUDRATE_1000KB    ( 1000 * 1000 )  

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

struct ua_can_ops;
typedef struct ua_can_bus
{
    struct ua_device parent;
    const struct ua_can_ops *ops;

    /** API */
    int (*send)(struct ua_can_bus *bus, ua_can_msg_p msg);
    int (*recv)(struct ua_can_bus *bus, ua_can_msg_p msg);
}ua_can_bus_t;
typedef ua_can_bus_t *ua_can_bus_p;

// struct can_configure
// {
//     uint32_t baud_rate;
//     uint8_t work_mode : 2;
//     uint8_t frame_type : 1;
//     uint8_t :5;
// };

typedef struct ua_can_ops
{
    // int (*configure)(ua_can_bus_p bus, struct can_configure *cfg);
    // int (*ctrl)(ua_can_bus_p bus, int cmd, void *arg);          // reserved
    int (*send)(ua_can_bus_p bus, ua_can_msg_p msg);
    int (*recv)(ua_can_bus_p bus, ua_can_msg_p msg);
}ua_can_ops_t;
typedef ua_can_ops_t *ua_can_ops_p;

int ua_can_bus_register(ua_can_bus_p bus, const char *name, ua_can_ops_p ops);

int ua_can_emit_rx_complete_event(ua_can_bus_p bus, uint32_t length);

#endif
