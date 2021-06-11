/*
 * File      : fal_cfg.h
 * This file is part of FAL (Flash Abstraction Layer) package
 * COPYRIGHT (C) 2006 - 2018, RT-Thread Development Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-05-17     armink       the first version
 */

#ifndef _FAL_CFG_H_
#define _FAL_CFG_H_


#define STM32_FLASH_START_ADRESS     ((uint32_t)0x08000000)
#define STM32_FLASH_SIZE             (64 * 1024)
#define STM32_FLASH_END_ADDRESS      ((uint32_t)(STM32_FLASH_START_ADRESS + STM32_FLASH_SIZE))


/* ===================== Flash device Configuration ========================= */
extern const struct fal_flash_dev stm32_onchip_flash;

/* flash device table */
#define FAL_FLASH_DEV_TABLE                                          \
{                                                                    \
    &stm32_onchip_flash,                                           \
}
/* ====================== Partition Configuration ========================== */

#define FAL_PART_HAS_TABLE_CFG

#if 1
/* partition table */
#define FAL_PART_TABLE                                                               \
{                                                                                    \
    {FAL_PART_MAGIC_WORD,        "bl",     "onchip_flash",         0,  32*1024, 0}, \
    {FAL_PART_MAGIC_WORD,    "otaMem",     "onchip_flash",   32*1024,   2*1024, 0}, \
    {FAL_PART_MAGIC_WORD,       "app",     "onchip_flash",   34*1024,  30*1024, 0}, \
}

#else 
/* partition table */
#define FAL_PART_TABLE                                                               \
{                                                                                    \
    {FAL_PART_MAGIC_WORD,        "bl",     "onchip_flash",         0,  26*1024, 0}, \
    {FAL_PART_MAGIC_WORD,    "otaMem",     "onchip_flash",   26*1024,   2*1024, 0}, \
    {FAL_PART_MAGIC_WORD,       "app",     "onchip_flash",   28*1024,  36*1024, 0}, \
}
#endif

#endif /* _FAL_CFG_H_ */
