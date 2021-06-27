/*****************************************************************************
 * @file    NULL
 * @brief   NULL
 * @details NULL
 * 
 * @author  GYC
 * @date    2021.5.24
 * @version 0.1
 * 
 * @todo    add support for all operations. add rx buffer
 * 
 * @note    only use tx0 rx0
 * 
 * @attention NULL
 * 
 *----------------------------------------------------------------------------
 *  Change History :                                                          
 *----------------------------------------------------------------------------
 *  Date       |  Version   |  Author         |  Description                     
 *-------------|------------|-----------------|---------------------------------
 *  2021.05.24 |  0.1       |  GYC            |  Create file                     
 * 
*******************************************************************************/
#include <stdint.h>
#include <string.h>
#include "mcp2515.h"
#include "mcp2515_hal.h"
#include "mcp2515_def.h"
#include "../cfg.h"
#include <ua_def.h>

enum {
    MCP2515_MODE_NORMAL     = 0,
    MCP2515_MODE_SLEEP      = 1,
    MCP2515_MODE_LOOPBACK   = 2,
    MCP2515_MODE_MONITOR    = 3,
    MCP2515_MODE_CONFIG     = 4,
};

#define SEND(buf, len)                          hal->send(hal, buf, len)
#define SEND_THEN_SEND(buf1, len1, buf2, len2)  hal->send_then_send(hal, buf1, len1, buf2, len2)
#define SEND_THEN_RECV(buf1, len1, buf2, len2)  hal->send_then_recv(hal, buf1, len1, buf2, len2)

/* ! extended-id format: stdID(11) + extentID(18)  (! stdID is in high */
struct eid2reg
{
    uint32_t id_eid0 : 8;
    uint32_t id_eid8 : 8;
    uint32_t id_sidl_eid : 2;
    uint32_t id_sidl_sid : 3;
    uint32_t id_sidh : 8;
    uint32_t : 3;
};

#if 0
struct can_frame {
    // uint8_t id_type;
    uint32_t id;
    uint8_t data[8];
};
#endif

/****** BASIC IO ******/
static void _cmd_reset(mcp2515_hal_t hal)
{
    uint8_t buf[] = {CMD_RESET};
    SEND(buf, sizeof(buf));
}
static void _cmd_read(mcp2515_hal_t hal, uint8_t addr, uint8_t *buffer, uint8_t len)
{
    uint8_t buf[] = {CMD_READ, addr};
    SEND_THEN_RECV(buf, sizeof(buf), buffer, len);
}
static void _cmd_write(mcp2515_hal_t hal, uint8_t addr, uint8_t *buffer, uint8_t len)
{
    uint8_t buf[] = {CMD_WRITE, addr};
    SEND_THEN_SEND(buf, sizeof(buf), buffer, len);
}
static void _cmd_read_rxbuf(mcp2515_hal_t hal, uint8_t addr, uint8_t *buffer, uint8_t len)
{
    uint8_t buf[] = {CMD_READ_RX_BUFFER};
    buf[0] |= ((addr & 3) << 1);
    SEND_THEN_RECV(buf, sizeof(buf), buffer, len);
}
static void _cmd_write_txbuf(mcp2515_hal_t hal, uint8_t addr, uint8_t *buffer, uint8_t len)
{
    uint8_t buf[] = {CMD_WRITE_TX_BUFFER};
    buf[0] |= (addr & 7);
    SEND_THEN_SEND(buf, sizeof(buf), buffer, len);
}
static void _cmd_rts(mcp2515_hal_t hal, uint8_t mask)
{
    uint8_t buf[] = {CMD_RTS};
    buf[0] |= (mask&7);
    SEND(buf, sizeof(buf));
}
static void _cmd_modify_bits(mcp2515_hal_t hal, uint8_t addr, uint8_t mask, uint8_t data)
{
    uint8_t buf[] = {CMD_MODIFY_BITS, addr, mask, data};
    SEND(buf, sizeof(buf));
}

/****** BASIC OP ******/
static void _set_mode(mcp2515_hal_t hal, uint8_t mode)
{
    //TODO
    _cmd_modify_bits(hal, REG_ADDR_CANCTRL, 0xE0, mode<<5);
#if 0
    //validate
    struct reg_CANSTAT buf;
    _cmd_read(hal, REG_ADDR_CANSTAT, (uint8_t *)&buf, 1);
    if (buf.OPMOD != mode) {
        printf("Set mode failed!\n");
        //wait, check again
    }
#endif
}

/**
 * @param regs  a buffer (size: 4)
 * @param id extended id
 * @param exide useless (1-extended 0-standard)
 */
static void _id_to_regs(uint8_t regs[], uint32_t id, uint8_t exide)
{
    //TODO
    struct eid2reg *_id = (struct eid2reg *)&id;
    struct reg_nSIDL *sidl = (struct reg_nSIDL *)&regs[1];

    regs[0] = _id->id_sidh;
    /* only support extended id now */
    sidl->EXIDE = 1;
    sidl->sid_lb = _id->id_sidl_sid;
    sidl->EID = _id->id_sidl_eid;
    regs[2] = _id->id_eid8;
    regs[3] = _id->id_eid0;
}
/**
 * @param regs regs (size: 4)
 * @param id extended id
 * @return 1: extended-id  0: standard-id
 */
static int _regs_to_id(const uint8_t regs[], uint32_t *id)
{
    struct eid2reg _id = {0};
    struct reg_nSIDL *sidl = (struct reg_nSIDL *)&regs[1];
    uint8_t exide;

    _id.id_sidh = regs[0];
    _id.id_sidl_sid = sidl->sid_lb;
    _id.id_sidl_eid = sidl->EID;
    exide = sidl->EXIDE;
    _id.id_eid8 = regs[2];
    _id.id_eid0 = regs[3];
    *id = *((uint32_t *)&_id);

    return exide;
}

static void _default_config_irq(mcp2515_hal_t hal)
{
    /* default: enabled: rx0 */
    _cmd_modify_bits(hal, REG_ADDR_CANINTE, 0xff, 1);
}

static void _default_cofig_canctrl(mcp2515_hal_t hal)
{
    /* defualt: sys/1, diabled: clkout, osm */
    _cmd_modify_bits(hal, REG_ADDR_CANCTRL, 0xF, 0);
}

static void _default_config_rx(mcp2515_hal_t hal)
{
    // only rxb0 now
    /* default: receive all frame (i.e no filters) */
    _cmd_modify_bits(hal, REG_ADDR_RXB0_BASE + REG_OFFSET_ADDR_BnCTRL, 0x60, 0x60);
}

static void _config_bitrate(mcp2515_hal_t hal, int bitrate, uint8_t freq_unit_m)
{
    //TODO
    // tbit = sjw + ps + ps1 + ps2
    // m1: 1, 1, 1, 2       (5)
    // m2: 1, 2, 2, 3       (8)
    uint8_t freq = 16;

    uint8_t buffer[3];
    struct reg_CNF1 *cnf1 = (struct reg_CNF1 *)&buffer[2];
    struct reg_CNF2 *cnf2 = (struct reg_CNF2 *)&buffer[1];
    struct reg_CNF3 *cnf3 = (struct reg_CNF3 *)&buffer[0];

    switch (bitrate)
    {
    case MCP2515_CAN_BITRATE_500K:
        // 16M / 8(1+2+2+3) / 4 = 500K
        cnf1->SJW = 0;    // 1TQ
        cnf1->BRP = 1;
        cnf2->BTLMODE = 1;
        cnf2->SAM = 0;
        cnf2->PHSEG1 = 1;
        cnf2->PRSEG = 1;
        cnf3->WAKFIL = 0;    //?
        cnf3->PHSEG2 = 2;
        break;

    case MCP2515_CAN_BITRATE_1000K:
        // 16M / 8(1+2+2+3) / 2 = 1M
        cnf1->SJW = 0;       // 1TQ
        cnf1->BRP = 0;
        cnf2->BTLMODE = 1;
        cnf2->SAM = 0;
        cnf2->PHSEG1 = 1;
        cnf2->PRSEG = 1;
        cnf3->WAKFIL = 0;    //?
        cnf3->PHSEG2 = 2;
        break;
    
    default:
        // rt_kprintf("Error. unsupported bitrate config\n");
        return;
        break;
    }

    _cmd_write(hal, REG_ADDR_CNF3, buffer, 3);
}

static void _config_irq(mcp2515_hal_t hal)
{
    //TODO
    uint8_t inte = 0;
    inte |= CANINTE_MASK_RX0IF;
    _cmd_modify_bits(hal, REG_ADDR_CANINTE, 0xFF, inte);

    _default_config_rx(hal);
}

int mcp2515_can_read(mcp2515_t ins, uint32_t *id, uint8_t *buf, uint8_t *len)
{
    //TODO
    uint8_t buffer[13];
    uint8_t exide;
    
    //it will clear irq flag 
    _cmd_read_rxbuf(&ins->hal, SUBCMD_BASE_RXB0SIDH, buffer, sizeof(buffer));
    
    //won't
    // _cmd_read(&ins->hal, REG_ADDR_RXB0_BASE + REG_OFFSET_ADDR_BnSIDH, buffer, sizeof(buffer));

    *len = buffer[4] & 0xF;
    memcpy(buf, &buffer[5], *len);
    exide = _regs_to_id(buffer, id);

    return UA_EOK;
}

/**
 * @brief called after irq or periodically calling. 
 * 
 * @param ins mcp2515 instance
 * @return int 
 */
int mcp2515_can_polling_read(mcp2515_t ins)
{
    struct reg_CANINTF intf;
    _cmd_read(&ins->hal, REG_ADDR_CANINTF, (uint8_t *)&intf, 1);
    
    if (intf.RX0IF) {
        uint32_t id;
        uint8_t data[8];
        uint8_t len;
        mcp2515_can_read(ins, &id, data, &len);
        ins->rx_cb(ins, id, data, len);
        //clear irq flag
        // _cmd_modify_bits(&ins->hal, REG_ADDR_CANINTF, CANINTF_MASK_RX0IF, 0);

        return UA_EOK;
    }

    return UA_EEMPTY;
}

/**
 * @brief send can data
 * 
 * @param id extended id
 * @param len size of buf (1 ~ 8)
 * @return successs: UA_EOK
 */
int mcp2515_can_send(mcp2515_t ins, uint32_t id, uint8_t *buf, uint8_t len)
{
    //TODO
    uint8_t regs[13];

    _id_to_regs(regs, id, 1);
    regs[4] = len & 0x0f;
    for (int i=0; i<len; i++) {
        regs[5+i] = buf[i];
    }
    _cmd_write_txbuf(&ins->hal, SUBCMD_BASE_TXB0SIDH, regs, 5+len);

    //rts txb0
    _cmd_rts(&ins->hal, 1);

    return UA_EOK;
}

#if 0
/**
 * @brief Used in irq mode
 */
void mcp2515_irq_callback(mcp2515_t ins)
{
    //TODO
    struct reg_CANINTF intf;
    _cmd_read(&ins->hal, REG_ADDR_CANINTF, (uint8_t *)&intf, 1);
    
    if (intf.RX0IF) {
        uint32_t id;
        uint8_t data[8];
        uint8_t len;

        mcp2515_can_read(ins, &id, data, &len);
        ins->rx_cb(ins, id, data, len);
        //clear irq flag.  the function "mcp2515_can_read" will clear irq flag
        //_cmd_modify_bits(&ins->hal, REG_ADDR_CANINTF, CANINTF_MASK_RX0IF, 0);
    }
    else {
        //TODO
    }
}
#endif

int mcp2515_init(mcp2515_t ins, mcp2515_rxcb_t rx_cb, void *param)
{
    //TODO
    int sta;
    sta = mcp2515_hal_init(&ins->hal, param);
    if (sta != UA_EOK) {
        return UA_ERROR;
    }

    ins->rx_cb = rx_cb;

    _cmd_reset(&ins->hal);

    _default_cofig_canctrl(&ins->hal);
    _config_bitrate(&ins->hal, CFG_DEFAULT_BITRATE, 16);

    // _default_config_rx(&ins->hal);
    _config_irq(&ins->hal);

    _set_mode(&ins->hal, MCP2515_MODE_NORMAL);

    return UA_EOK;
}
