#include <stdio.h>
#include <ua_def.h>
#include <elog.h>

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
    log_a("hi");
    log_e("hi");
    log_d("hi");
    log_i("hi");
    log_v("hi");
    log_w("hi");
}

int main()
{
    void *__init_array_start  = 0;
    unsigned long addr[8];
    int cnt = 0;

    init_elog();
    test_elog();
    

    board_init();
}