
/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2006-04-30     Bernard      first implementation
 * 2006-05-04     Bernard      add list_thread,
 *                                 list_sem,
 *                                 list_timer
 * 2006-05-20     Bernard      add list_mutex,
 *                                 list_mailbox,
 *                                 list_msgqueue,
 *                                 list_event,
 *                                 list_fevent,
 *                                 list_mempool
 * 2006-06-03     Bernard      display stack information in list_thread
 * 2006-08-10     Bernard      change version to invoke rt_show_version
 * 2008-09-10     Bernard      update the list function for finsh syscall
 *                                 list and sysvar list
 * 2009-05-30     Bernard      add list_device
 * 2010-04-21     yi.qiu       add list_module
 * 2012-04-29     goprife      improve the command line auto-complete feature.
 * 2012-06-02     lgnq         add list_memheap
 * 2012-10-22     Bernard      add MS VC++ patch.
 * 2016-06-02     armink       beautify the list_thread command
 * 2018-11-22     Jesven       list_thread add smp support
 * 2018-12-27     Jesven       Fix the problem that disable interrupt too long in list_thread
 *                             Provide protection for the "first layer of objects" when list_*
 * 2020-04-07     chenhui      add clear
 */

// #include <rthw.h>
// #include <rtthread.h>

#include "./cfg.h"

#ifdef RT_USING_FINSH

#include "finsh.h"

#define LIST_FIND_OBJ_NR 8

long hello(void)
{
    ua_printf("Hello RT-Thread!\n");

    return 0;
}
FINSH_FUNCTION_EXPORT(hello, say hello world);
MSH_CMD_EXPORT(hello,clear the terminal screen);

static long clear(void)
{
    ua_printf("\x1b[2J\x1b[H");

    return 0;
}
FINSH_FUNCTION_EXPORT(clear,clear the terminal screen);
MSH_CMD_EXPORT(clear,clear the terminal screen);

extern void rt_show_version(void);
long version(void)
{
    rt_show_version();

    return 0;
}
FINSH_FUNCTION_EXPORT(version, show RT-Thread version information);
MSH_CMD_EXPORT(version, show RT-Thread version information);

// rt_inline void object_split(int len)
// {
//     while (len--) ua_printf("-");
// }

// typedef struct
// {
//     rt_list_t *list;
//     rt_list_t **array;
//     uint8_t type;
//     int nr;             /* input: max nr, can't be 0 */
//     int nr_out;         /* out: got nr */
// } list_get_next_t;

// static void list_find_init(list_get_next_t *p, uint8_t type, rt_list_t **array, int nr)
// {
//     struct rt_object_information *info;
//     rt_list_t *list;

//     info = rt_object_get_information((enum rt_object_class_type)type);
//     list = &info->object_list;

//     p->list = list;
//     p->type = type;
//     p->array = array;
//     p->nr = nr;
//     p->nr_out = 0;
// }

// static rt_list_t *list_get_next(rt_list_t *current, list_get_next_t *arg)
// {
//     int first_flag = 0;
//     rt_ubase_t level;
//     rt_list_t *node, *list;
//     rt_list_t **array;
//     int nr;

//     arg->nr_out = 0;

//     if (!arg->nr || !arg->type)
//     {
//         return (rt_list_t *)RT_NULL;
//     }

//     list = arg->list;

//     if (!current) /* find first */
//     {
//         node = list;
//         first_flag = 1;
//     }
//     else
//     {
//         node = current;
//     }

//     // level = rt_hw_interrupt_disable();

//     if (!first_flag)
//     {
//         struct rt_object *obj;
//         /* The node in the list? */
//         obj = rt_list_entry(node, struct rt_object, list);
//         if ((obj->type & ~RT_Object_Class_Static) != arg->type)
//         {
//             rt_hw_interrupt_enable(level);
//             return (rt_list_t *)RT_NULL;
//         }
//     }

//     nr = 0;
//     array = arg->array;
//     while (1)
//     {
//         node = node->next;

//         if (node == list)
//         {
//             node = (rt_list_t *)RT_NULL;
//             break;
//         }
//         nr++;
//         *array++ = node;
//         if (nr == arg->nr)
//         {
//             break;
//         }
//     }

//     // rt_hw_interrupt_enable(level);
//     arg->nr_out = nr;
//     return node;
// }

long list(void)
{
#ifndef FINSH_USING_MSH_ONLY
    struct finsh_syscall_item *syscall_item;
    struct finsh_sysvar_item *sysvar_item;
#endif

    ua_printf("--Function List:\n");
    {
        struct finsh_syscall *index;
        for (index = _syscall_table_begin;
                index < _syscall_table_end;
                FINSH_NEXT_SYSCALL(index))
        {
            /* skip the internal command */
            if (strncmp((char *)index->name, "__", 2) == 0) continue;

#if defined(FINSH_USING_DESCRIPTION) && defined(FINSH_USING_SYMTAB)
            ua_printf("%-16s -- %s\n", index->name, index->desc);
#else
            ua_printf("%s\n", index->name);
#endif
        }
    }

#ifndef FINSH_USING_MSH_ONLY
    /* list syscall list */
    syscall_item = global_syscall_list;
    while (syscall_item != NULL)
    {
        ua_printf("[l] %s\n", syscall_item->syscall.name);
        syscall_item = syscall_item->next;
    }

    ua_printf("--Variable List:\n");
    {
        struct finsh_sysvar *index;
        for (index = _sysvar_table_begin;
                index < _sysvar_table_end;
                FINSH_NEXT_SYSVAR(index))
        {
#ifdef FINSH_USING_DESCRIPTION
            ua_printf("%-16s -- %s\n", index->name, index->desc);
#else
            ua_printf("%s\n", index->name);
#endif
        }
    }

    sysvar_item = global_sysvar_list;
    while (sysvar_item != NULL)
    {
        ua_printf("[l] %s\n", sysvar_item->sysvar.name);
        sysvar_item = sysvar_item->next;
    }
#endif

    return 0;
}
FINSH_FUNCTION_EXPORT(list, list all symbol in system)
MSH_CMD_EXPORT(list, list all symbol in system)

int msh_help(int argc, char **argv)
{
    ua_printf("RT-Thread shell commands:\n");
    {
        struct finsh_syscall *index;

        for (index = _syscall_table_begin;
                index < _syscall_table_end;
                FINSH_NEXT_SYSCALL(index))
        {
            if (strncmp(index->name, "__cmd_", 6) != 0) continue;
#if defined(FINSH_USING_DESCRIPTION) && defined(FINSH_USING_SYMTAB)
            ua_printf("%-16s - %s\n", &index->name[6], index->desc);
#else
            ua_printf("%s ", &index->name[6]);
#endif
        }
    }
    ua_printf("\n");

    return 0;
}
MSH_CMD_EXPORT_ALIAS(msh_help, help, RT-Thread shell help.);

#ifndef FINSH_USING_MSH_ONLY
static int str_is_prefix(const char *prefix, const char *str)
{
    while ((*prefix) && (*prefix == *str))
    {
        prefix ++;
        str ++;
    }

    if (*prefix == 0)
        return 0;

    return -1;
}

static int str_common(const char *str1, const char *str2)
{
    const char *str = str1;

    while ((*str != 0) && (*str2 != 0) && (*str == *str2))
    {
        str ++;
        str2 ++;
    }

    return (str - str1);
}

void list_prefix(char *prefix)
{
    struct finsh_syscall_item *syscall_item;
    struct finsh_sysvar_item *sysvar_item;
    uint16_t func_cnt, var_cnt;
    int length, min_length;
    const char *name_ptr;

    func_cnt = 0;
    var_cnt  = 0;
    min_length = 0;
    name_ptr = RT_NULL;

    /* checks in system function call */
    {
        struct finsh_syscall *index;
        for (index = _syscall_table_begin;
                index < _syscall_table_end;
                FINSH_NEXT_SYSCALL(index))
        {
            /* skip internal command */
            if (str_is_prefix("__", index->name) == 0) continue;

            if (str_is_prefix(prefix, index->name) == 0)
            {
                if (func_cnt == 0)
                {
                    ua_printf("--function:\n");

                    if (*prefix != 0)
                    {
                        /* set name_ptr */
                        name_ptr = index->name;

                        /* set initial length */
                        min_length = strlen(name_ptr);
                    }
                }

                func_cnt ++;

                if (*prefix != 0)
                {
                    length = str_common(name_ptr, index->name);
                    if (length < min_length)
                        min_length = length;
                }

#ifdef FINSH_USING_DESCRIPTION
                ua_printf("%-16s -- %s\n", index->name, index->desc);
#else
                ua_printf("%s\n", index->name);
#endif
            }
        }
    }

    /* checks in dynamic system function call */
    syscall_item = global_syscall_list;
    while (syscall_item != NULL)
    {
        if (str_is_prefix(prefix, syscall_item->syscall.name) == 0)
        {
            if (func_cnt == 0)
            {
                ua_printf("--function:\n");
                if (*prefix != 0 && name_ptr == NULL)
                {
                    /* set name_ptr */
                    name_ptr = syscall_item->syscall.name;

                    /* set initial length */
                    min_length = strlen(name_ptr);
                }
            }

            func_cnt ++;

            if (*prefix != 0)
            {
                length = str_common(name_ptr, syscall_item->syscall.name);
                if (length < min_length)
                    min_length = length;
            }

            ua_printf("[l] %s\n", syscall_item->syscall.name);
        }
        syscall_item = syscall_item->next;
    }

    /* checks in system variable */
    {
        struct finsh_sysvar *index;
        for (index = _sysvar_table_begin;
                index < _sysvar_table_end;
                FINSH_NEXT_SYSVAR(index))
        {
            if (str_is_prefix(prefix, index->name) == 0)
            {
                if (var_cnt == 0)
                {
                    ua_printf("--variable:\n");

                    if (*prefix != 0 && name_ptr == NULL)
                    {
                        /* set name_ptr */
                        name_ptr = index->name;

                        /* set initial length */
                        min_length = strlen(name_ptr);

                    }
                }

                var_cnt ++;

                if (*prefix != 0)
                {
                    length = str_common(name_ptr, index->name);
                    if (length < min_length)
                        min_length = length;
                }

#ifdef FINSH_USING_DESCRIPTION
                ua_printf("%-16s -- %s\n", index->name, index->desc);
#else
                ua_printf("%s\n", index->name);
#endif
            }
        }
    }

    /* checks in dynamic system variable */
    sysvar_item = global_sysvar_list;
    while (sysvar_item != NULL)
    {
        if (str_is_prefix(prefix, sysvar_item->sysvar.name) == 0)
        {
            if (var_cnt == 0)
            {
                ua_printf("--variable:\n");
                if (*prefix != 0 && name_ptr == NULL)
                {
                    /* set name_ptr */
                    name_ptr = sysvar_item->sysvar.name;

                    /* set initial length */
                    min_length = strlen(name_ptr);
                }
            }

            var_cnt ++;

            if (*prefix != 0)
            {
                length = str_common(name_ptr, sysvar_item->sysvar.name);
                if (length < min_length)
                    min_length = length;
            }

            ua_printf("[v] %s\n", sysvar_item->sysvar.name);
        }
        sysvar_item = sysvar_item->next;
    }

    /* only one matched */
    if (name_ptr != NULL)
    {
        rt_strncpy(prefix, name_ptr, min_length);
    }
}
#endif

#if defined(FINSH_USING_SYMTAB) && !defined(FINSH_USING_MSH_ONLY)
static int dummy = 0;
FINSH_VAR_EXPORT(dummy, finsh_type_int, dummy variable for finsh)
#endif

#endif /* RT_USING_FINSH */

