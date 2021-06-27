/*****************************************************************************
 * @file    NULL
 * @brief   NULL
 * @details NULL
 * 
 * @author  GYC
 * @date    2021.5.24
 * @version 0.1
 * 
 * @todo    add support for all operations
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
#ifndef __DEF_CMD_H__
#define __DEF_CMD_H__

enum cmd{
    CMD_RESET           = 0xC0,
    CMD_READ            = 0x03,
    CMD_READ_RX_BUFFER  = 0x90, //
    CMD_WRITE           = 0x02,
    CMD_WRITE_TX_BUFFER = 0x40, //
    CMD_RTS             = 0x80,
    CMD_READ_STATUS     = 0xA0,
    CMD_READ_RX_STATUS  = 0xB0,
    CMD_MODIFY_BITS     = 0x05,
};

enum subcmd_read_rx_buffer{
    SUBCMD_BASE_RXB0SIDH = 0,
    SUBCMD_BASE_RXB0D0,
    SUBCMD_BASE_RXB1SIDH,
    SUBCMD_BASE_RXB1D0,
};

enum subcmd_write_tx_buffer{
    SUBCMD_BASE_TXB0SIDH = 0,
    SUBCMD_BASE_TXB0D0,
    SUBCMD_BASE_TXB1SIDH = 0,
    SUBCMD_BASE_TXB1D0,
    SUBCMD_BASE_TXB2SIDH = 0,
    SUBCMD_BASE_TXB2D0,
};

#endif //__DEF_CMD_H__
