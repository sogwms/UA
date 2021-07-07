#include <ua.h>
#include <hal_can.h>

#define LOG_TAG "test-can-bus"
#include <elog.h>


static int event_handler(ua_device_p dev, int event, void *content)
{
    if (event == UA_DEVICE_EVENT_RX_COMPLETE) {
        uint32_t length = *(uint32_t *)content;
        log_i("event rx-complete. size:%d", length);

        ua_can_msg_t msg;
        ua_can_recv((ua_can_bus_p)dev, &msg);
        log_d("Msg readed from can: frameType: %s, idType: %s, id: 0x%x, dataLen: %d", 
            msg.frame_type == UA_CAN_FRAME_TYPE_DATA ? "DATA" : "REMOTE",
            msg.id_type == UA_CAN_ID_TYPE_STD ? "STD" : "EXT",
            msg.id, msg.data_length);
    }
}

int test_dev_can_init(void)
{
    ua_can_bus_p dev = (ua_can_bus_p)ua_device_find("can1");

    if (dev == NULL) {
        log_a("Find device failed");
        return UA_ERROR;
    }

    ua_device_set_callback((ua_device_p)dev, event_handler);

    ua_can_msg_t msg;
    msg.frame_type = UA_CAN_FRAME_TYPE_DATA;
    msg.id_type = UA_CAN_ID_TYPE_EXT;
    msg.id = 0x66;
    msg.data_length = 8;

    ua_can_send(dev, &msg);

    return UA_EOK;
}

UA_EXPORT_INIT_APP(test_dev_can_init);
