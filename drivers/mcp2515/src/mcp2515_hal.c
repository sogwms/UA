/*****************************************************************************
 * @file    NULL
 * @brief   NULL
 * @details NULL
 * 
 * @author  GYC
 * @date    2021.5.24
 * @version 0.1
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
 * 
*******************************************************************************/
#include <spi.h>
#include <gpio.h>
#include "mcp2515_hal.h"
#include <hal_spi.h>
#include <bsp_spi.h>

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
    // char *dev_name = (char *)param;
    hal->send = send;
    hal->send_then_recv = send_then_recv;
    hal->send_then_send = send_then_send;

    static struct bsp_spi_cs cs;
    ua_spi_device_p spi_dev;

    cs.GPIO_Pin = GPIO_PIN_6;
    cs.GPIOx = GPIOE;
    bsp_hw_spi_attach_device("spi20", "spi2", &cs);

    spi_dev = (ua_spi_device_p)ua_device_find("spi20");

    hal->dev = spi_dev;

    return UA_EOK;
}
