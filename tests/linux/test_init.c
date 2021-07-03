#include <ua_def.h>
#include <stdio.h>

#define LOG_TAG "test-init"
#include <elog.h>

static int init_begin(void)
{
    log_i("Here is init begin\n");
    return UA_EOK;
}
UA_EXPORT_INIT(init_begin, "0");

static int init_board_begin(void)
{
    log_i("Here is init board begin\n");
    return UA_EOK;
}
UA_EXPORT_INIT(init_board_begin, "0.end");

static int init_prev_begin(void)
{
    log_i("Here is init prev begin\n");
    return UA_EOK;
}
UA_EXPORT_INIT(init_prev_begin, "1.end");

static int init_end(void)
{
    log_i("Here is init end\n");
    return UA_EOK;
}
UA_EXPORT_INIT(init_end, "6.end");

int test_init()
{
    const struct ua_init_cb *pcb = NULL;

    // extern const int __ua_init_start;
    // extern const int __ua_init_end;

    for (pcb=&__ua__init_init_begin; pcb<=&__ua__init_init_end;) {
        log_i("Going to run \"%s\"", pcb->fn_name);
        pcb->fn();
        pcb++;
    }

    return UA_EOK;
}