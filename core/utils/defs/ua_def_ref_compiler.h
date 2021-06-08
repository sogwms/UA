#ifndef __UA_DEF_REF_COMPILER_H__
#define __UA_DEF_REF_COMPILER_H__

#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
#define __CLANG_ARM
#endif

/* Compiler Related Definitions */
#if defined(__CC_ARM) || defined(__CLANG_ARM)           /* ARM Compiler */
    #include <stdarg.h>
    #define UA_SECTION(x)               __attribute__((section(x)))
    #define UA_UNUSED                   __attribute__((unused))
    #define UA_USED                     __attribute__((used))
    #define ALIGN(n)                    __attribute__((aligned(n)))

    #define UA_WEAK                     __attribute__((weak))
    #define ua_inline                   static __inline
    /* module compiling */
    #ifdef UA_USING_MODULE
        #define UAT_API                 __declspec(dllimport)
    #else
        #define UAT_API                 __declspec(dllexport)
    #endif

#elif defined (__IAR_SYSTEMS_ICC__)     /* for IAR Compiler */
    #include <stdarg.h>
    #define UA_SECTION(x)               @ x
    #define UA_UNUSED
    #define UA_USED                     __root
    #define PRAGMA(x)                   _Pragma(#x)
    #define ALIGN(n)                    PRAGMA(data_alignment=n)
    #define UA_WEAK                     __weak
    #define ua_inline                   static inline
    #define UAT_API

#elif defined (__GNUC__)                /* GNU GCC Compiler */
    #ifdef UA_USING_NEWLIB
        #include <stdarg.h>
    #else
        /* the version of GNU GCC must be greater than 4.x */
        typedef __builtin_va_list       __gnuc_va_list;
        typedef __gnuc_va_list          va_list;
        #define va_start(v,l)           __builtin_va_start(v,l)
        #define va_end(v)               __builtin_va_end(v)
        #define va_arg(v,l)             __builtin_va_arg(v,l)
    #endif

    #define UA_SECTION(x)               __attribute__((section(x)))
    #define UA_UNUSED                   __attribute__((unused))
    #define UA_USED                     __attribute__((used))
    #define ALIGN(n)                    __attribute__((aligned(n)))
    #define UA_WEAK                     __attribute__((weak))
    #define ua_inline                   static __inline
    #define UAT_API
#elif defined (__ADSPBLACKFIN__)        /* for VisualDSP++ Compiler */
    #include <stdarg.h>
    #define UA_SECTION(x)               __attribute__((section(x)))
    #define UA_UNUSED                   __attribute__((unused))
    #define UA_USED                     __attribute__((used))
    #define ALIGN(n)                    __attribute__((aligned(n)))
    #define UA_WEAK                     __attribute__((weak))
    #define ua_inline                   static inline
    #define UAT_API
#elif defined (_MSC_VER)
    #include <stdarg.h>
    #define UA_SECTION(x)
    #define UA_UNUSED
    #define UA_USED
    #define ALIGN(n)                    __declspec(align(n))
    #define UA_WEAK
    #define ua_inline                   static __inline
    #define UAT_API
#elif defined (__TI_COMPILER_VERSION__)
    #include <stdarg.h>
    /* The way that TI compiler set section is different from other(at least
     * GCC and MDK) compilers. See ARM Optimizing C/C++ Compiler 5.9.3 for more
     * details. */
    #define UA_SECTION(x)
    #define UA_UNUSED
    #define UA_USED
    #define PRAGMA(x)                   _Pragma(#x)
    #define ALIGN(n)
    #define UA_WEAK
    #define ua_inline                   static inline
    #define UAT_API
#elif defined (__TASKING__)
    #include <stdarg.h>
    #define UA_SECTION(x)               __attribute__((section(x)))
    #define UA_UNUSED                   __attribute__((unused))
    #define UA_USED                     __attribute__((used, protect))
    #define PRAGMA(x)                   _Pragma(#x)
    #define ALIGN(n)                    __attribute__((__align(n)))
    #define UA_WEAK                     __attribute__((weak))
    #define ua_inline                   static inline
    #define UAT_API
#else
    #error not supported tool chain
#endif

#endif // __UA_DEF_REF_COMPILER_H__