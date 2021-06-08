#ifndef __BSP_SPI_H__
#define __BSP_SPI_H__

#include <gpio.h>

struct bsp_spi_cs
{
    GPIO_TypeDef *GPIOx;
    uint16_t GPIO_Pin;
};

int bsp_hw_spi_attach_device(const char *name, const char *bus_name, struct bsp_spi_cs *cs);

#endif // 
