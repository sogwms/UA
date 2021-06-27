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
#ifndef __MCP2515_HAL_H__
#define __MCP2515_HAL_H__

typedef struct mcp2515_hal *mcp2515_hal_t;

struct mcp2515_hal {
    void *dev;
    int (*send_then_recv)(mcp2515_hal_t hal, void *send_buf, int send_len, void *recv_buf, int recv_len);
    int (*send)(mcp2515_hal_t hal, void *send_buf, int len);
    int (*send_then_send)(mcp2515_hal_t hal, void *send_buf1, int len1, void *send_buf2, int len2);
};

int mcp2515_hal_init(struct mcp2515_hal *hal, void *param);

#endif //__MCP2515_HAL_H__
