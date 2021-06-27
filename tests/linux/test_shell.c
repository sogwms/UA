#include <finsh.h>

extern void finsh_thread_entry(void *parameter);
extern int finsh_system_init(void);
extern void finsh_system_function_init(const void *begin, const void *end);

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
    printf("Echo: test %d\n", argc);
}
MSH_CMD_EXPORT(test, list all symbol in system)

int test_shell_routine()
{
    finsh_system_init();
    finsh_thread_entry(0);
}
