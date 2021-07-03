#include <stdint.h>
#include <stdio.h>

void edf_elog_port_output(const char *log, uint32_t size)
{
    printf("%.*s", (int)size, log);
}
const char *edf_elog_port_get_time(void)
{
    return "TIME";
}
const char *edf_elog_port_get_p_info(void)
{
    return "PROCESS";
}
const char *edf_elog_port_get_t_info(void)
{
    return "THREAD";
}