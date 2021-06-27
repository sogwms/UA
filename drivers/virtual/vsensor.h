#define DEF_VSENSOR_CTRL_CMD(prefix) {     \
    prefix##VSENSOR_CTRL_CMD_SET_BITRATE,  \
    prefix##VSENSOR_CTRL_CMD_SHUTDOWN,     \
    prefix##VSENSOR_CTRL_CMD_SHUTDOWN2,     \
}

enum vsensor_ctrl_cmd DEF_VSENSOR_CTRL_CMD();