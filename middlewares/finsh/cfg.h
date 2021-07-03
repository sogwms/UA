#ifndef __CFG_H__
#define __CFG_H__

#include <ua_def.h>
#include "port_support.h"

#define RT_USING_FINSH

#define FINSH_USING_DESCRIPTION

#define FINSH_USING_SYMTAB
//*
//"Using module shell"
#define FINSH_USING_MSH
//"Using module shell in default"
#define FINSH_USING_MSH_DEFAULT
//"Only using module shell"
#define FINSH_USING_MSH_ONLY

//"The command arg num for shell"
#define FINSH_ARG_MAX 10


#define RT_ALIGN_SIZE 4

#define RT_CONSOLEBUF_SIZE 128

#define RT_ASSERT(e)

#endif // !__CFG_H__