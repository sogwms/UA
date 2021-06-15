#include <stdio.h>
#include <ua_def.h>

extern int bsp_spi_init(void);
extern int test_elog_init();
extern int test_shell_routine(void);
extern int test_init();

void board_init(void) 
{
    bsp_spi_init();
}

int main()
{
    test_elog_init();

    board_init();

    test_init();

    test_shell_routine();
}
