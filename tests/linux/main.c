#include <stdio.h>
#include <ua_def.h>

extern int bsp_spi_init(void);

void board_init(void) 
{
    bsp_spi_init();
}

int main()
{
    unsigned long addr[8];
    int cnt = 0;
    board_init();
}