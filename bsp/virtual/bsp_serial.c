#include <hal_serial_impl.h>
#include <string.h>

#define LOG_TAG "bsp-serial"
#include <elog.h>

#define BUFFER_SIZE 1024

static uint8_t data_buffer[BUFFER_SIZE];
static uint32_t data_length = 0;

static int ctrl(ua_serial_p dev, int cmd, void *arg)
{
    return UA_ERROR;
}

static int send(ua_serial_p dev, const void *data, uint32_t len)
{
    log_d("received send request. size: %d", len);

    if (len > BUFFER_SIZE) return 0;

    memcpy(data_buffer, data, len);
    data_length = len;

    // loopback
    log_d("loopback emit rx-complete event");
    ua_serial_rx_complete(dev, len);

    return len;
}

static int recv(ua_serial_p dev, void *data, uint32_t len)
{
    log_d("received recv request. size: %d", len);
    
    if (len > data_length) len = data_length;

    memcpy(data, data_buffer, len);

    return len;
}

static ua_serial_ops_t _ops = {
    .ctrl = ctrl,
    .send = send,
    .recv = recv,
};

static ua_serial_t _serial;

int bsp_serial_init(void)
{
    return ua_serial_register(&_serial, "serial1", &_ops);
}

UA_EXPORT_INIT_BOARD(bsp_serial_init);
