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
#ifndef __MCP2515_H__
#define __MCP2515_H__

#include <stdint.h>
#include "mcp2515_hal.h"

enum {
    MCP2515_CAN_BITRATE_500K,
    MCP2515_CAN_BITRATE_1000K,
};

typedef struct mcp2515 *mcp2515_t;
typedef void (*mcp2515_rxcb_t)(struct mcp2515 *ins, uint32_t id, uint8_t *data, uint8_t len);
struct mcp2515 {
    struct mcp2515_hal hal;
    mcp2515_rxcb_t rx_cb;

    // void (*rx_ind)(struct mcp2515 *hal);
};

int mcp2515_init(mcp2515_t ins, mcp2515_rxcb_t rx_cb, void *param);
int mcp2515_can_send(mcp2515_t ins, uint32_t id, uint8_t *buf, uint8_t len);
int mcp2515_can_polling_read(mcp2515_t ins);

#endif // __MCP2515_H__
