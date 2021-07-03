#ifndef _PORT_SUPPORT_H_
#define _PORT_SUPPORT_H_

#include <stdint.h>

typedef int                             rt_bool_t;      /**< boolean type */
typedef long                            rt_base_t;      /**< Nbit CPU related date type */
typedef unsigned long                   rt_ubase_t;     /**< Nbit unsigned CPU related data type */

typedef rt_base_t                       rt_err_t;       /**< Type for error number */
typedef rt_ubase_t                      rt_size_t;      /**< Type for size number */

void *rt_memmove(void *dest, const void *src, rt_ubase_t n);
char *rt_strncpy(char *dst, const char *src, rt_ubase_t n);
#endif // !_PORT_SUPPORT_H_
