#include <ua_def.h>
#include <stdio.h>

static int init_board_begin(void)
{
    printf("board begin\n");
    return UA_EOK;
}
UA_EXPORT_INIT(init_board_begin, "0.end");

static int init_board_end(void)
{
    printf("board end\n");
    return UA_EOK;
}
UA_EXPORT_INIT(init_board_end, "1.end");

static int init_begin(void)
{
    printf("init begin\n");
    return UA_EOK;
}
UA_EXPORT_INIT(init_begin, "0");

static int init_end(void)
{
    printf("init end\n");
    return UA_EOK;
}
UA_EXPORT_INIT(init_end, "6.end");

static int test(void)
{
    printf("test func init\n");
    return UA_EOK;
}
UA_EXPORT_INIT_BOARD(test);

int test_init()
{
    const struct ua_init_cb *pcb = NULL;

    // extern const int __ua_init_start;
    // extern const int __ua_init_end;

    for (pcb=&__ua__init_init_begin; pcb<&__ua__init_init_end;) {
        pcb++;
        pcb->fn();
    }

    return UA_EOK;
}