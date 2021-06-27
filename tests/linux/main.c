#include <stdio.h>
#include <ua_def.h>
#include <elog.h>


#include <stdio.h>
#include <finsh.h>

extern void finsh_thread_entry(void *parameter);
extern int finsh_system_init(void);
extern void finsh_system_function_init(const void *begin, const void *end);


extern int bsp_spi_init(void);

void board_init(void) 
{
    bsp_spi_init();
}


static void init_elog(void)
{
  elog_init();
  /* set EasyLogger log format */
  elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_ALL);
  elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
  elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
  elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
  elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_ALL & ~(ELOG_FMT_FUNC | ELOG_FMT_T_INFO | ELOG_FMT_P_INFO));
  elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_ALL & ~(ELOG_FMT_FUNC | ELOG_FMT_T_INFO | ELOG_FMT_P_INFO));
  
  /* enable color */
  elog_set_text_color_enabled(true);
  /* start EasyLogger */
  elog_start();
}

static void test_elog()
{
    log_a("Hi");
    log_e("Hi");
    log_d("Hi");
    log_i("Hi");
    log_v("Hi");
    log_w("Hi");
}

int main()
{
    void *__init_array_start  = 0;
    unsigned long addr[8];
    int cnt = 0;

    init_elog();
    test_elog();
    

    board_init();

    finsh_system_init();
    printf("Hi\n");
    finsh_thread_entry(0);
}



char rt_hw_console_getchar(void)
{
    char c = getchar();
    return c;
}

void rt_show_version(void)
{
    printf("Here should print version info\n");
}

int test(int argc, char **argv)
{
    printf("test");
}
MSH_CMD_EXPORT(test, list all symbol in system)
