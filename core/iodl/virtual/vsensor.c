#include <ioal.h>
#include "vsensor.h"

typedef int (*ctrl_cmd_response_function)(ua_device_p dev, int cmd, void *args);

int __VSENSOR_CTRL_CMD_SET_BITRATE(ua_device_p dev, int cmd, void *args)
{

}
int __VSENSOR_CTRL_CMD_SHUTDOWN(ua_device_p dev, int cmd, void *args)  
{

}
int __VSENSOR_CTRL_CMD_SHUTDOWN2(ua_device_p dev, int cmd, void *args) 
{

}

ctrl_cmd_response_function ctrl_cmd_response_funcs[] = DEF_VSENSOR_CTRL_CMD(__);

static void deal_ctrl_cmd(ua_device_p dev, int cmd, void *args)
{
    sizeof(ctrl_cmd_response_funcs);
    ctrl_cmd_response_funcs[cmd](dev, cmd, args);
}
