#include <ua.h>
#include <hal_serial.h>

#define LOG_TAG "test-serial-device"
#include <elog.h>

#define DATA_SIZE 256

static int event_handler(ua_device_p dev, int event, void *content)
{
    if (event == UA_DEVICE_EVENT_RX_COMPLETE) {
        uint32_t length = *(uint32_t *)content;
        uint8_t buffer[length];
        log_i("event rx-complete. size:%d", length);
        ua_serial_recv((ua_serial_p)dev, buffer, length);
    }
}

int test_dev_serial_init(void)
{
    ua_serial_p dev = (ua_serial_p)ua_device_find("serial1");

    if (dev == NULL) {
        log_a("Find device failed");
        return UA_ERROR;
    }

    ua_device_set_callback((ua_device_p)dev, event_handler);

    uint8_t data[DATA_SIZE];

    for (int i=0; i<DATA_SIZE; i++) {
        data[i] = i;
    }
    ua_serial_send(dev, data, DATA_SIZE);

    return UA_EOK;
}

UA_EXPORT_INIT_APP(test_dev_serial_init);
