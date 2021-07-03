#include <stdio.h>
#include <ua_def.h>

extern int test_elog_init();
extern int test_shell_routine(void);
extern int test_init();

int main()
{
    test_elog_init();

    test_init();

    // it won't back, keep running at the end
    test_shell_routine();
}
