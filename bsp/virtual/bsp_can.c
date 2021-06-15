#include <hal_can.h>

#define LOG_TAG "bsp-can"
#include <elog.h>

static int configure(ua_can_bus_p bus, struct can_configure *cfg) 
{
    log_v("configure");
    return UA_EOK;
}

static int ctrl(ua_can_bus_p bus, int cmd, void *arg) 
{
    log_v("ctrl: (%d", cmd);
    return UA_EOK;
}

static int send(ua_can_bus_p bus, const void *buf, int len) 
{
    log_v("send: (%d", len);
    return UA_EOK;
}

static int recv(ua_can_bus_p bus, void *buf, int len) 
{
    log_v("recv: (%d", len);
    return UA_EOK;
}

static ua_can_ops_t _can_ops = {
    .configure = configure,
    .ctrl = ctrl,
    .send = send,
    .recv = recv,
};

static ua_can_bus_t _bus;

int bsp_can_init(void)
{
    ua_can_bus_register(&_bus, "can1", &_can_ops);
}
