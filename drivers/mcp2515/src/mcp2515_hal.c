/*****************************************************************************
 * @file    NULL
 * @brief   NULL
 * @details NULL
 * 
 * @author  GYC
 * @date    2021.07.08
 * @version 0.2
 * 
 * @todo    NULL
 * 
 * @note    NULL
 * 
 * @attention   NULL
 * 
 *----------------------------------------------------------------------------
 *  Change History :                                                          
 *----------------------------------------------------------------------------
 *  Date       |  Version   |  Author         |  Description                     
 *-------------|------------|-----------------|---------------------------------
 *  2021.05.24 |  0.1       |  GYC            |  Create file                     
 *  2021.07.08 |  0.2       |  GYC            |  Optimize portability
*******************************************************************************/
#include "mcp2515_hal.h"
#include <hal_spi.h>
#include <string.h>

#define LOG_TAG "mcp2515-hal"
#include <elog.h>

static int send_then_recv(mcp2515_hal_t hal, void *send_buf, int send_len, void *recv_buf, int recv_len)
{
    return ua_spi_send_then_recv((ua_spi_device_p)hal->dev, send_buf, send_len, recv_buf, recv_len);
}
static int send(mcp2515_hal_t hal, void *send_buf, int len)
{
    return ua_spi_send((ua_spi_device_p)hal->dev, send_buf, len);
}
static int send_then_send(mcp2515_hal_t hal, void *send_buf1, int len1, void *send_buf2, int len2)
{
    return ua_spi_send_then_send((ua_spi_device_p)hal->dev, send_buf1, len1, send_buf2, len2);
}

int mcp2515_hal_init(struct mcp2515_hal *hal, void *param)
{
    char *dev_name = (char *)param;

    memset(hal, 0, sizeof(struct mcp2515_hal));

    ua_device_p device = ua_device_find(dev_name);
    if (device == NULL) {
        log_a("Failed to find device on %s", dev_name);
        return UA_ERROR;
    }

    if (!ua_device_check_class(device, UA_DEVICE_CLASS_SPI_DEVICE)) {
        log_a("Wrong device class!");
        return UA_ERROR;
    }

    hal->dev = device;
    hal->send = send;
    hal->send_then_recv = send_then_recv;
    hal->send_then_send = send_then_send;

    return UA_EOK;
}
