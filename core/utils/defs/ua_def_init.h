
#define UA_USING_EXPORT_INIT
/* initialization export */
#ifdef UA_USING_EXPORT_INIT
typedef int (*init_fn_t)(void);
#ifdef _MSC_VER 
    #define UA_EXPORT_INIT(fn, level)
#else
    #define UA_EXPORT_INIT(fn, level)  UA_USED const init_fn_t __ua__init_##fn  UA_SECTION(".uai_fn." level) = fn
#endif
#else
    #define UA_EXPORT_INIT(fn, level)
#endif

/* board init routines will be called in board_init() function */
#define UA_EXPORT_INIT_BOARD(fn)           UA_EXPORT_INIT(fn, "1")

/* pre/device/component/env/app init routines will be called in init_thread */
/* components pre-initialization (pure software initilization) */
#define UA_EXPORT_INIT_PREV(fn)            UA_EXPORT_INIT(fn, "2")
/* device initialization */
#define UA_EXPORT_INIT_DEVICE(fn)          UA_EXPORT_INIT(fn, "3")
/* components initialization (dfs, lwip, ...) */
#define UA_EXPORT_INIT_COMPONENT(fn)       UA_EXPORT_INIT(fn, "4")
/* environment initialization (mount disk, ...) */
#define UA_EXPORT_INIT_ENV(fn)             UA_EXPORT_INIT(fn, "5")
/* appliation initialization (rtgui application etc ...) */
#define UA_EXPORT_INIT_APP(fn)             UA_EXPORT_INIT(fn, "6")
