#ifndef __RT_DEF_H__
#define __RT_DEF_H__

/* boolean type definitions */
#define RT_TRUE                         1               /**< boolean true  */
#define RT_FALSE                        0               /**< boolean fails */

#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
#define __CLANG_ARM
#endif

/* Compiler Related Definitions */
#if defined(__CC_ARM) || defined(__CLANG_ARM)           /* ARM Compiler */
    #include <stdarg.h>
    #define RT_SECTION(x)               __attribute__((section(x)))
    #define RT_UNUSED                   __attribute__((unused))
    #define RT_USED                     __attribute__((used))
    #define ALIGN(n)                    __attribute__((aligned(n)))

    #define RT_WEAK                     __attribute__((weak))
    #define rt_inline                   static __inline
    /* module compiling */
    #ifdef RT_USING_MODULE
        #define RTT_API                 __declspec(dllimport)
    #else
        #define RTT_API                 __declspec(dllexport)
    #endif

#elif defined (__IAR_SYSTEMS_ICC__)     /* for IAR Compiler */
    #include <stdarg.h>
    #define RT_SECTION(x)               @ x
    #define RT_UNUSED
    #define RT_USED                     __root
    #define PRAGMA(x)                   _Pragma(#x)
    #define ALIGN(n)                    PRAGMA(data_alignment=n)
    #define RT_WEAK                     __weak
    #define rt_inline                   static inline
    #define RTT_API

#elif defined (__GNUC__)                /* GNU GCC Compiler */
    #ifdef RT_USING_NEWLIB
        #include <stdarg.h>
    #else
        /* the version of GNU GCC must be greater than 4.x */
        typedef __builtin_va_list       __gnuc_va_list;
        typedef __gnuc_va_list          va_list;
        #define va_start(v,l)           __builtin_va_start(v,l)
        #define va_end(v)               __builtin_va_end(v)
        #define va_arg(v,l)             __builtin_va_arg(v,l)
    #endif

    #define RT_SECTION(x)               __attribute__((section(x)))
    #define RT_UNUSED                   __attribute__((unused))
    #define RT_USED                     __attribute__((used))
    #define ALIGN(n)                    __attribute__((aligned(n)))
    #define RT_WEAK                     __attribute__((weak))
    #define PACKED                      __attribute__((packed))
    #define rt_inline                   static __inline
    #define RTT_API
#elif defined (__ADSPBLACKFIN__)        /* for VisualDSP++ Compiler */
    #include <stdarg.h>
    #define RT_SECTION(x)               __attribute__((section(x)))
    #define RT_UNUSED                   __attribute__((unused))
    #define RT_USED                     __attribute__((used))
    #define ALIGN(n)                    __attribute__((aligned(n)))
    #define RT_WEAK                     __attribute__((weak))
    #define rt_inline                   static inline
    #define RTT_API
#elif defined (_MSC_VER)
    #include <stdarg.h>
    #define RT_SECTION(x)
    #define RT_UNUSED
    #define RT_USED
    #define ALIGN(n)                    __declspec(align(n))
    #define RT_WEAK
    #define rt_inline                   static __inline
    #define RTT_API
#elif defined (__TI_COMPILER_VERSION__)
    #include <stdarg.h>
    /* The way that TI compiler set section is different from other(at least
     * GCC and MDK) compilers. See ARM Optimizing C/C++ Compiler 5.9.3 for more
     * details. */
    #define RT_SECTION(x)
    #define RT_UNUSED
    #define RT_USED
    #define PRAGMA(x)                   _Pragma(#x)
    #define ALIGN(n)
    #define RT_WEAK
    #define rt_inline                   static inline
    #define RTT_API
#elif defined (__TASKING__)
    #include <stdarg.h>
    #define RT_SECTION(x)               __attribute__((section(x)))
    #define RT_UNUSED                   __attribute__((unused))
    #define RT_USED                     __attribute__((used, protect))
    #define PRAGMA(x)                   _Pragma(#x)
    #define ALIGN(n)                    __attribute__((__align(n)))
    #define RT_WEAK                     __attribute__((weak))
    #define rt_inline                   static inline
    #define RTT_API
#else
    #error not supported tool chain
#endif

/* initialization export */
#ifdef RT_USING_COMPONENTS_INIT
typedef int (*init_fn_t)(void);
#ifdef _MSC_VER /* we do not support MS VC++ compiler */
#pragma section("rti_fn$f",read)
    #if RT_DEBUG_INIT
        struct rt_init_desc
        {
            const char* level;
            const init_fn_t fn;
            const char* fn_name;
        };
        #define INIT_EXPORT(fn, level)                                  \
                                const char __rti_level_##fn[] = level"__rt_init_"#fn;   \
                                const char __rti_##fn##_name[] = #fn;                   \
                                __declspec(allocate("rti_fn$f"))                        \
                                RT_USED const struct rt_init_desc __rt_init_msc_##fn =  \
                                {__rti_level_##fn, fn, __rti_##fn##_name};
    #else
        struct rt_init_desc
        {
            const char* level;
            const init_fn_t fn;
        };
        #define INIT_EXPORT(fn, level)                                  \
                                const char __rti_level_##fn[] = level"__rt_init_"#fn;   \
                                __declspec(allocate("rti_fn$f"))                        \
                                RT_USED const struct rt_init_desc __rt_init_msc_##fn =  \
                                {__rti_level_##fn, fn };
    #endif
#else
    #if RT_DEBUG_INIT
        struct rt_init_desc
        {
            const char* fn_name;
            const init_fn_t fn;
        };
        #define INIT_EXPORT(fn, level)                                                       \
            const char __rti_##fn##_name[] = #fn;                                            \
            RT_USED const struct rt_init_desc __rt_init_desc_##fn RT_SECTION(".rti_fn." level) = \
            { __rti_##fn##_name, fn};
    #else
        #define INIT_EXPORT(fn, level)                                                       \
            RT_USED const init_fn_t __rt_init_##fn RT_SECTION(".rti_fn." level) = fn
    #endif
#endif
#else
#define INIT_EXPORT(fn, level)
#endif

/* board init routines will be called in board_init() function */
#define INIT_BOARD_EXPORT(fn)           INIT_EXPORT(fn, "1")

/* pre/device/component/env/app init routines will be called in init_thread */
/* components pre-initialization (pure software initilization) */
#define INIT_PREV_EXPORT(fn)            INIT_EXPORT(fn, "2")
/* device initialization */
#define INIT_DEVICE_EXPORT(fn)          INIT_EXPORT(fn, "3")
/* components initialization (dfs, lwip, ...) */
#define INIT_COMPONENT_EXPORT(fn)       INIT_EXPORT(fn, "4")
/* environment initialization (mount disk, ...) */
#define INIT_ENV_EXPORT(fn)             INIT_EXPORT(fn, "5")
/* appliation initialization (rtgui application etc ...) */
#define INIT_APP_EXPORT(fn)             INIT_EXPORT(fn, "6")

// #if !defined(RT_USING_FINSH)
// /* define these to empty, even if not include finsh.h file */
// #define FINSH_FUNCTION_EXPORT(name, desc)
// #define FINSH_FUNCTION_EXPORT_ALIAS(name, alias, desc)
// #define FINSH_VAR_EXPORT(name, type, desc)

// #define MSH_CMD_EXPORT(command, desc)
// #define MSH_CMD_EXPORT_ALIAS(command, alias, desc)
// #elif !defined(FINSH_USING_SYMTAB)
// #define FINSH_FUNCTION_EXPORT_CMD(name, cmd, desc)
// #endif

/**
 * @addtogroup Error
 */

/**@{*/

/* RT-Thread error code definitions */
#define RT_EOK                          0               /**< There is no error */
#define RT_ERROR                        1               /**< A generic error happens */
#define RT_ETIMEOUT                     2               /**< Timed out */
#define RT_EFULL                        3               /**< The resource is full */
#define RT_EEMPTY                       4               /**< The resource is empty */
#define RT_ENOMEM                       5               /**< No memory */
#define RT_ENOSYS                       6               /**< No system */
#define RT_EBUSY                        7               /**< Busy */
#define RT_EIO                          8               /**< IO error */
#define RT_EINTR                        9               /**< Interrupted system call */
#define RT_EINVAL                       10              /**< Invalid argument */

/**@}*/

/**
 * @ingroup BasicDef
 *
 * @def RT_ALIGN(size, align)
 * Return the most contiguous size aligned at specified width. RT_ALIGN(13, 4)
 * would return 16.
 */
#define RT_ALIGN(size, align)           (((size) + (align) - 1) & ~((align) - 1))

/**
 * @ingroup BasicDef
 *
 * @def RT_ALIGN_DOWN(size, align)
 * Return the down number of aligned at specified width. RT_ALIGN_DOWN(13, 4)
 * would return 12.
 */
#define RT_ALIGN_DOWN(size, align)      ((size) & ~((align) - 1))

/**
 * @ingroup BasicDef
 *
 * @def RT_NULL
 * Similar as the \c NULL in C library.
 */
#define RT_NULL                         (0)



/**
 * Double List structure
 */
struct rt_list_node
{
    struct rt_list_node *next;                          /**< point to next node. */
    struct rt_list_node *prev;                          /**< point to prev node. */
};
typedef struct rt_list_node rt_list_t;                  /**< Type for lists. */

/**
 * Single List structure
 */
struct rt_slist_node
{
    struct rt_slist_node *next;                         /**< point to next node. */
};
typedef struct rt_slist_node rt_slist_t;                /**< Type for single list. */


#endif
