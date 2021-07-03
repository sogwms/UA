/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2013-03-30     Bernard      the first verion for finsh
 * 2014-01-03     Bernard      msh can execute module.
 * 2017-07-19     Aubr.Cool    limit argc to RT_FINSH_ARG_MAX
 */
// #include <rtthread.h>

#include "./cfg.h"

#ifdef FINSH_USING_MSH

#include "msh.h"
#include <finsh.h>
#include "shell.h"

#ifdef RT_USING_DFS
#include <dfs_posix.h>
#endif

#ifdef RT_USING_MODULE
#include <dlmodule.h>
#endif

#ifndef FINSH_ARG_MAX
#define FINSH_ARG_MAX    8
#endif

typedef int (*cmd_function_t)(int argc, char **argv);

#ifdef FINSH_USING_MSH
#ifdef FINSH_USING_MSH_ONLY
bool msh_is_used(void)
{
    return true;
}
#else
#ifdef FINSH_USING_MSH_DEFAULT
static bool __msh_state = true;
#else
static bool __msh_state = false;
#endif
bool msh_is_used(void)
{
    return __msh_state;
}

static int msh_exit(int argc, char **argv)
{
    /* return to finsh shell mode */
    __msh_state = false;
    return 0;
}
FINSH_FUNCTION_EXPORT_ALIAS(msh_exit, __cmd_exit, return to RT-Thread shell mode.);

static int msh_enter(void)
{
    /* enter module shell mode */
    __msh_state = true;
    return 0;
}
FINSH_FUNCTION_EXPORT_ALIAS(msh_enter, msh, use module shell);
#endif

extern int msh_help(int argc, char **argv);

// int msh_help(int argc, char **argv)
// {
//     ua_printf("RT-Thread shell commands:\n");
//     {
//         struct finsh_syscall *index;

//         for (index = _syscall_table_begin;
//                 index < _syscall_table_end;
//                 FINSH_NEXT_SYSCALL(index))
//         {
//             if (strncmp(index->name, "__cmd_", 6) != 0) continue;
// #if defined(FINSH_USING_DESCRIPTION) && defined(FINSH_USING_SYMTAB)
//             ua_printf("%-16s - %s\n", &index->name[6], index->desc);
// #else
//             ua_printf("%s ", &index->name[6]);
// #endif
//         }
//     }
//     ua_printf("\n");

//     return 0;
// }
// FINSH_FUNCTION_EXPORT_ALIAS(msh_help, __cmd_help, RT-Thread shell help.);  GYC

// int cmd_ps(int argc, char **argv)
// {
//     extern long list_thread(void);
//     extern int list_module(void);

// #ifdef RT_USING_MODULE
//     if ((argc == 2) && (strcmp(argv[1], "-m") == 0))
//         list_module();
    // else
// #endif
//         list_thread();
//     return 0;
// }
// FINSH_FUNCTION_EXPORT_ALIAS(cmd_ps, __cmd_ps, List threads in the system.);

static int msh_split(char *cmd, rt_size_t length, char *argv[FINSH_ARG_MAX])
{
    char *ptr;
    rt_size_t position;
    rt_size_t argc;
    rt_size_t i;

    ptr = cmd;
    position = 0; argc = 0;

    while (position < length)
    {
        /* strip bank and tab */
        while ((*ptr == ' ' || *ptr == '\t') && position < length)
        {
            *ptr = '\0';
            ptr ++; position ++;
        }

        if(argc >= FINSH_ARG_MAX)
        {
            ua_printf("Too many args ! We only Use:\n");
            for(i = 0; i < argc; i++)
            {
                ua_printf("%s ", argv[i]);
            }
            ua_printf("\n");
            break;
        }

        if (position >= length) break;

        /* handle string */
        if (*ptr == '"')
        {
            ptr ++; position ++;
            argv[argc] = ptr; argc ++;

            /* skip this string */
            while (*ptr != '"' && position < length)
            {
                if (*ptr == '\\')
                {
                    if (*(ptr + 1) == '"')
                    {
                        ptr ++; position ++;
                    }
                }
                ptr ++; position ++;
            }
            if (position >= length) break;

            /* skip '"' */
            *ptr = '\0'; ptr ++; position ++;
        }
        else
        {
            argv[argc] = ptr;
            argc ++;
            while ((*ptr != ' ' && *ptr != '\t') && position < length)
            {
                ptr ++; position ++;
            }
            if (position >= length) break;
        }
    }

    return argc;
}

static cmd_function_t msh_get_cmd(char *cmd, int size)
{
    struct finsh_syscall *index;
    cmd_function_t cmd_func = NULL;

    for (index = _syscall_table_begin;
            index < _syscall_table_end;
            FINSH_NEXT_SYSCALL(index))
    {
        // ua_printf("run in\n");
        // ua_printf("add:%x\n", index);
        // ua_printf("index->name:%s\n",index->name);
        // ua_printf("run in2\n");

        if (strncmp(index->name, "__cmd_", 6) != 0) continue;
        // ua_printf("run in22\n");

        if (strncmp(&index->name[6], cmd, size) == 0 &&
                index->name[6 + size] == '\0')
        {
        // ua_printf("run in3\n");

            cmd_func = (cmd_function_t)index->func;
            break;
        }
    }

    return cmd_func;
}

static int _msh_exec_cmd(char *cmd, rt_size_t length, int *retp)
{
    int argc;
    rt_size_t cmd0_size = 0;
    cmd_function_t cmd_func;
    char *argv[FINSH_ARG_MAX];

    RT_ASSERT(cmd);
    RT_ASSERT(retp);

    /* find the size of first command */
    while ((cmd[cmd0_size] != ' ' && cmd[cmd0_size] != '\t') && cmd0_size < length)
        cmd0_size ++;
    if (cmd0_size == 0)
        return -UA_ERROR;

    cmd_func = msh_get_cmd(cmd, cmd0_size);
    if (cmd_func == NULL)
        return -UA_ERROR;

    /* split arguments */
    memset(argv, 0x00, sizeof(argv));
    argc = msh_split(cmd, length, argv);
    if (argc == 0)
        return -UA_ERROR;

    /* exec this command */
    *retp = cmd_func(argc, argv);
    return 0;
}

int msh_exec(char *cmd, rt_size_t length)
{
    int cmd_ret;

    /* strim the beginning of command */
    while (*cmd  == ' ' || *cmd == '\t')
    {
        cmd++;
        length--;
    }

    if (length == 0)
        return 0;

    /* Exec sequence:
     * 1. built-in command
     * 2. module(if enabled)
     */
    if (_msh_exec_cmd(cmd, length, &cmd_ret) == 0)
    {
        return cmd_ret;
    }
#ifdef RT_USING_DFS
#ifdef DFS_USING_WORKDIR
    if (msh_exec_script(cmd, length) == 0)
    {
        return 0;
    }
#endif

#ifdef RT_USING_MODULE
    if (msh_exec_module(cmd, length) == 0)
    {
        return 0;
    }
#endif

#ifdef RT_USING_LWP
    if (_msh_exec_lwp(cmd, length) == 0)
    {
        return 0;
    }
#endif
#endif

    /* truncate the cmd at the first space. */
    {
        char *tcmd;
        tcmd = cmd;
        while (*tcmd != ' ' && *tcmd != '\0')
        {
            tcmd++;
        }
        *tcmd = '\0';
    }
    ua_printf("%s: command not found.\n", cmd);
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

void msh_auto_complete(char *prefix)
{
    int length, min_length;
    const char *name_ptr, *cmd_name;
    struct finsh_syscall *index;

    min_length = 0;
    name_ptr = NULL;

    if (*prefix == '\0')
    {
        msh_help(0, NULL);
        return;
    }

    /* checks in internal command */
    {
        for (index = _syscall_table_begin; index < _syscall_table_end; FINSH_NEXT_SYSCALL(index))
        {
            /* skip finsh shell function */
            if (strncmp(index->name, "__cmd_", 6) != 0) continue;

            cmd_name = (const char *) &index->name[6];
            if (strncmp(prefix, cmd_name, strlen(prefix)) == 0)
            {
                if (min_length == 0)
                {
                    /* set name_ptr */
                    name_ptr = cmd_name;
                    /* set initial length */
                    min_length = strlen(name_ptr);
                }

                length = str_common(name_ptr, cmd_name);
                if (length < min_length)
                    min_length = length;

                ua_printf("%s\n", cmd_name);
            }
        }
    }

    /* auto complete string */
    if (name_ptr != NULL)
    {
        rt_strncpy(prefix, name_ptr, min_length);
    }

    return ;
}
#endif

#endif /* FINSH_USING_MSH */
