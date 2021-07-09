#include <string.h>
#include <hal_can.h>

#define LOG_TAG "bsp-can"
#include <elog.h>

#define BUFFER_SIZE 16

static ua_can_msg_t g_data_buffer[BUFFER_SIZE];
static uint32_t g_data_length = 0;

static int send(ua_can_bus_p bus, ua_can_msg_p msg)
{
    log_d("Received send request: frameType: %s, idType: %s, id: 0x%x, dataLen: %d", 
        msg->frame_type == UA_CAN_FRAME_TYPE_DATA ? "DATA" : "REMOTE",
        msg->id_type == UA_CAN_ID_TYPE_STD ? "STD" : "EXT",
        msg->id, msg->data_length);

    memcpy(&g_data_buffer[g_data_length], msg, sizeof(ua_can_msg_t));
    g_data_length++;

    log_d("Internal loopback. emit rx-complete event");
    ua_can_emit_rx_complete_event(bus, 1);

    return UA_EOK;
}

static int recv(ua_can_bus_p bus, ua_can_msg_p msg) 
{
    log_d("Received recv request");

    if (g_data_length == 0) return UA_EEMPTY;

    g_data_length--;
    memcpy(msg, &g_data_buffer[g_data_length], sizeof(ua_can_msg_t));

    return UA_EOK;
}

static ua_can_ops_t _can_ops = {
    // .configure = configure,
    // .ctrl = ctrl,
    .send = send,
    .recv = recv,
};

static ua_can_bus_t _bus;

int bsp_can_init(void)
{
    return ua_can_bus_register(&_bus, "can1", &_can_ops);
}

UA_EXPORT_INIT_BOARD(bsp_can_init);
