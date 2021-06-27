/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-12-5      SummerGift   first version
 * 2020-03-05     redoc        support stm32f103vg
 *
 */
#define LOG_TAG                "drv.flash"
#include <elog.h>

#include "main.h"
#include <fal.h>
#include <ua_def.h>


/**
  * @brief  Gets the page of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The page of a given address
  */
static uint32_t GetPage(uint32_t addr)
{
    uint32_t page = 0;
    page = UA_ALIGN_DOWN(addr, FLASH_PAGE_SIZE);
    return page;
}

/**
 * Read data from flash.
 * @note This operation's units is word.
 *
 * @param addr flash address
 * @param buf buffer to store read data
 * @param size read bytes size
 *
 * @return result
 */
int stm32_flash_read(uint32_t addr, uint8_t *buf, size_t size)
{
    size_t i;

    if ((addr + size) > STM32_FLASH_END_ADDRESS)
    {
        log_e("read outrange flash size! addr is (0x%p)", (void *)(addr + size));
        return -UA_EINVAL;
    }

    for (i = 0; i < size; i++, buf++, addr++)
    {
        *buf = *(uint8_t *) addr;
    }

    return size;
}

/**
 * Write data to flash.
 * @note This operation's units is word.
 * @note This operation must after erase. @see flash_erase.
 *
 * @param addr flash address
 * @param buf the write data buffer
 * @param size write bytes size
 *
 * @return result
 */
int stm32_flash_write(uint32_t addr, const uint8_t *buf, size_t size)
{
    int result        = UA_EOK;
    uint32_t end_addr   = addr + size;

    if (addr % 4 != 0)
    {
        log_e("write addr must be 4-byte alignment");
        return -UA_EINVAL;
    }

    if ((end_addr) > STM32_FLASH_END_ADDRESS)
    {
        log_e("write outrange flash size! addr is (0x%p)", (void *)(addr + size));
        return -UA_EINVAL;
    }

    HAL_FLASH_Unlock();

    while (addr < end_addr)
    {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr, *((uint32_t *)buf)) == HAL_OK)
        {
            if (*(uint32_t *)addr != *(uint32_t *)buf)
            {
                result = -UA_ERROR;
                break;
            }
            addr += 4;
            buf  += 4;
        }
        else
        {
            result = -UA_ERROR;
            break;
        }
    }

    HAL_FLASH_Lock();

    if (result != UA_EOK)
    {
        return result;
    }

    return size;
}

/**
 * Erase data on flash with bank.
 * @note This operation is irreversible.
 * @note This operation's units is different which on many chips.
 *
 * @param bank flash bank
 * @param addr flash address
 * @param size erase bytes size
 *
 * @return result
 */
int stm32_flash_erase_bank(uint32_t bank, uint32_t addr, size_t size)
{
    int result = UA_EOK;
    uint32_t PAGEError = 0;

    /*Variable used for Erase procedure*/
    FLASH_EraseInitTypeDef EraseInitStruct;

    if ((addr + size) > STM32_FLASH_END_ADDRESS)
    {
        log_e("ERROR: erase outrange flash size! addr is (0x%p)\n", (void *)(addr + size));
        return -UA_EINVAL;
    }

    HAL_FLASH_Unlock();

    /* Fill EraseInit structure*/
    EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.PageAddress = GetPage(addr);
    EraseInitStruct.NbPages     = (size + FLASH_PAGE_SIZE - 1) / FLASH_PAGE_SIZE;
    EraseInitStruct.Banks       = bank;

    if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
    {
        result = -UA_ERROR;
        goto __exit;
    }

__exit:
    HAL_FLASH_Lock();

    if (result != UA_EOK)
    {
        return result;
    }

    log_d("erase done: addr (0x%p), size %d", (void *)addr, size);
    return size;
}

/**
 * Erase data on flash .
 * @note This operation is irreversible.
 * @note This operation's units is different which on many chips.
 *
 * @param addr flash address
 * @param size erase bytes size
 *
 * @return result
 */
int stm32_flash_erase(uint32_t addr, size_t size)
{
#if defined(FLASH_BANK2_END)
    int result = UA_EOK;
    uint32_t addr_bank1 = 0;
    size_t size_bank1 = 0;
    uint32_t addr_bank2 = 0;
    size_t size_bank2 = 0;

    if((addr + size) <= FLASH_BANK1_END)
    {
        addr_bank1 = addr;
        size_bank1 = size;
        size_bank2 = 0;
    }
    else if(addr > FLASH_BANK1_END)
    {
        size_bank1 = 0;
        addr_bank2 = addr;
        size_bank2 = size;
    }
    else
    {
        addr_bank1 = addr;
        size_bank1 = FLASH_BANK1_END + 1 - addr_bank1;
        addr_bank2 = FLASH_BANK1_END + 1;
        size_bank2 = addr + size - (FLASH_BANK1_END + 1);
    }

    if(size_bank1)
    {
        log_d("bank1: addr (0x%p), size %d", (void *)addr_bank1, size_bank1);
        if(size_bank1 != stm32_flash_erase_bank(FLASH_BANK_1, addr_bank1, size_bank1))
        {
            result = -UA_ERROR;
            goto __exit;
        }
    }

    if(size_bank2)
    {
        log_d("bank2: addr (0x%p), size %d", (void *)addr_bank2, size_bank2);
        if(size_bank2 != stm32_flash_erase_bank(FLASH_BANK_2, addr_bank2, size_bank2))
        {
            result = -UA_ERROR;
            goto __exit;
        }
    }

__exit:
    if(result != UA_EOK)
    {
        return result;
    }

    return size_bank1 + size_bank2;
#else
    return stm32_flash_erase_bank(FLASH_BANK_1, addr, size);
#endif
}

static int fal_flash_read(long offset, uint8_t *buf, size_t size);
static int fal_flash_write(long offset, const uint8_t *buf, size_t size);
static int fal_flash_erase(long offset, size_t size);

const struct fal_flash_dev stm32_onchip_flash = { "onchip_flash", STM32_FLASH_START_ADRESS, STM32_FLASH_SIZE, FLASH_PAGE_SIZE, {NULL, fal_flash_read, fal_flash_write, fal_flash_erase} };

static int fal_flash_read(long offset, uint8_t *buf, size_t size)
{
    return stm32_flash_read(stm32_onchip_flash.addr + offset, buf, size);
}

static int fal_flash_write(long offset, const uint8_t *buf, size_t size)
{
    return stm32_flash_write(stm32_onchip_flash.addr + offset, buf, size);
}

static int fal_flash_erase(long offset, size_t size)
{
    return stm32_flash_erase(stm32_onchip_flash.addr + offset, size);
}

