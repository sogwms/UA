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
#ifndef __DEF_REG_H__
#define __DEF_REG_H__

#include <stdint.h>

enum reg_addr {
    //tx buffer
    REG_ADDR_TXB0_BASE = 0x30,
    REG_ADDR_TXB1_BASE = 0x40,
    REG_ADDR_TXB2_BASE = 0x50,
    //rx buffer
    REG_ADDR_RXB0_BASE = 0x60,
    REG_ADDR_RXB1_BASE = 0x70,
    //rx filter
    REG_ADDR_RXF0_BASE = 0x00,
    REG_ADDR_RXF1_BASE = 0x04,
    REG_ADDR_RXF2_BASE = 0x08,
    REG_ADDR_RXF3_BASE = 0x10,
    REG_ADDR_RXF4_BASE = 0x14,
    REG_ADDR_RXF5_BASE = 0x18,
    //rx mask
    REG_ADDR_RXM0_BASE = 0x20,
    REG_ADDR_RXM1_BASE = 0x40,
    //error
    REG_ADDR_TEC  = 0x1C,
    REG_ADDR_REC  = 0x1D,
    REG_ADDR_EFLG = 0x2D,
    //irq
    REG_ADDR_CANINTE = 0x2B,
    REG_ADDR_CANINTF = 0x2C,
    //mode
    REG_ADDR_CANSTAT = 0x0E,    //xEh
    REG_ADDR_CANCTRL = 0x0F,    //xFh
    //
    REG_ADDR_CNF1 = 0x2A,
    REG_ADDR_CNF2 = 0x29,
    REG_ADDR_CNF3 = 0x28,
    //
    REG_ADDR_TXnRTS = 0x0D,
    REG_ADDR_RXnBF  = 0x0C,

};

/**RXFn; RXMn*/
enum reg_offset_addr_RXFn_RXMn {
    REG_OFFSET_ADDR_FMnSIDH = 0,
    REG_OFFSET_ADDR_FMnSIDL,
    REG_OFFSET_ADDR_FMnEID8,
    REG_OFFSET_ADDR_FMnEID0,
};

/**TXBn base:(0x30,0x40,0x50); RXBn base:(0x60,0x70);*/
enum reg_offset_addr_TXBn_RXBn{
    REG_OFFSET_ADDR_BnCTRL = 0,
    REG_OFFSET_ADDR_BnSIDH,
    REG_OFFSET_ADDR_BnSIDL,
    REG_OFFSET_ADDR_BnEID8,
    REG_OFFSET_ADDR_BnEID0,
    REG_OFFSET_ADDR_BnDLC,
    REG_OFFSET_ADDR_BnDm0,
    REG_OFFSET_ADDR_BnDm1,
    REG_OFFSET_ADDR_BnDm2,
    REG_OFFSET_ADDR_BnDm3,
    REG_OFFSET_ADDR_BnDm4,
    REG_OFFSET_ADDR_BnDm5,
    REG_OFFSET_ADDR_BnDm6,
    REG_OFFSET_ADDR_BnDm7,
};

enum reg_CANINTF_mask {
    CANINTF_MASK_RX0IF = 0x01,
    CANINTF_MASK_RX1IF = 0x02,
    CANINTF_MASK_TX0IF = 0x04,
    CANINTF_MASK_TX1IF = 0x08,
    CANINTF_MASK_TX2IF = 0x10,
    CANINTF_MASK_ERRIF = 0x20,
    CANINTF_MASK_WAKIF = 0x40,
    CANINTF_MASK_MERRF = 0x80,
};

enum reg_CANINTE_mask {
    CANINTE_MASK_RX0IF = 0x01,
    CANINTE_MASK_RX1IF = 0x02,
    CANINTE_MASK_TX0IF = 0x04,
    CANINTE_MASK_TX1IF = 0x08,
    CANINTE_MASK_TX2IF = 0x10,
    CANINTE_MASK_ERRIF = 0x20,
    CANINTE_MASK_WAKIF = 0x40,
    CANINTE_MASK_MERRF = 0x80,
};

//offset: 0
struct reg_TXBnCTRL 
{
    uint8_t TXP     : 2;
    uint8_t : 1;                
    uint8_t TXREQ   : 1;        
    uint8_t TXERR   : 1;        
    uint8_t MLOA    : 1;        
    uint8_t ABTF    : 1;        
    uint8_t : 1;                
};

// 60h
struct reg_RXB0CTRL
{
    uint8_t FILHIT  : 1;
    uint8_t BUKT1   : 1;
    uint8_t BUKT    : 1;
    uint8_t RXRTR   : 1;
    uint8_t : 1;
    uint8_t RXM     : 2;
    uint8_t : 1;
};
// 70h
struct reg_RXB1CTRL
{
    uint8_t FILHIT  : 3;
    uint8_t RXRTR   : 1;
    uint8_t : 1;
    uint8_t RXM     : 2;
    uint8_t : 1;
};


struct reg_nSIDH 
{
    uint8_t sid_hb;             //<10:3>
};
struct reg_nSIDL 
{
    uint8_t EID     : 2;        //<17:16>
    uint8_t : 1;
    uint8_t EXIDE   : 1;    
    uint8_t SRR:    1;          //useless in tx
    uint8_t sid_lb  : 3;        //<2:0>
};
struct reg_nEID8 
{
    uint8_t eid_hb;             //<15:8>
};
struct reg_nEID0 
{
    uint8_t eid_lb;             //<7:0>
};
struct reg_nDLC
{
    uint8_t DLC     : 4;        // 0 ~ 8
    uint8_t : 2;
    uint8_t RTR     : 1;
    uint8_t : 1;             
};


// 0Dh
struct reg_TXnRTS 
{
    uint8_t B0RTSM  : 1; 
    uint8_t B1RTSM  : 1;     
    uint8_t B2RTSM  : 1;     
    uint8_t B0RTS   : 1;     
    uint8_t B1RTS   : 1;     
    uint8_t B2RTS   : 1;     
    uint8_t : 2;           
};
// 0Ch
struct reg_RXnBF
{
    uint8_t B0BFM   : 1;
    uint8_t B1BFM   : 1;
    uint8_t B0BFE   : 1;
    uint8_t B1BFE   : 1;
    uint8_t B0BFS   : 1;
    uint8_t B1BFS   : 1;
    uint8_t : 2;
};


/****** ERROR DEF ******/
struct reg_EFLG 
{
    uint8_t EWARN   : 1;
    uint8_t RXWAR   : 1;
    uint8_t TXWAR   : 1;
    uint8_t RXEP    : 1;
    uint8_t TXEP    : 1;
    uint8_t TXBO    : 1;
    uint8_t RX0OVR  : 1;
    uint8_t RX1OVR  : 1;
};

/****** IRQ DEF ******/
struct reg_CANINTE
{
    uint8_t RX0IE   : 1;
    uint8_t RX1IE   : 1;
    uint8_t TX0IE   : 1;
    uint8_t TX1IE   : 1;
    uint8_t TX2IE   : 1;
    uint8_t ERRIE   : 1;
    uint8_t WAKIE   : 1;
    uint8_t MERRE   : 1;
};
struct reg_CANINTF
{
    uint8_t RX0IF   : 1;
    uint8_t RX1IF   : 1;
    uint8_t TX0IF   : 1;
    uint8_t TX1IF   : 1;
    uint8_t TX2IF   : 1;
    uint8_t ERRIF   : 1;
    uint8_t WAKIF   : 1;
    uint8_t MERRF   : 1;
};

/****** MODE DEF ******/
struct reg_CANCTRL
{
    uint8_t CLKPRE  : 2;
    uint8_t CLKEN   : 1;
    uint8_t OSM     : 1;
    uint8_t ABAT    : 1;
    uint8_t REQOP   : 3;
};

struct reg_CANSTAT
{
    uint8_t : 1;
    uint8_t ICOD    : 4;
    uint8_t : 1;
    uint8_t OPMOD   : 4;
};


/****** BAUDRATE DEF ******/
struct reg_CNF1 
{
    uint8_t BRP     : 6;
    uint8_t SJW     : 2;
};
struct reg_CNF2
{
    uint8_t PRSEG   : 3;
    uint8_t PHSEG1  : 3;
    uint8_t SAM     : 1;
    uint8_t BTLMODE : 1;
};
struct reg_CNF3 
{
    uint8_t PHSEG2  : 3;
    uint8_t : 3;
    uint8_t WAKFIL  : 1;
    uint8_t SOF     : 1;
};

#endif //
