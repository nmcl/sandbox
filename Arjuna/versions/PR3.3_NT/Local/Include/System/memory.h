/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: memory.h,v 1.3 1995/07/03 14:29:23 ngdp Exp $
 */

#ifndef MEMORY_H_
#define MEMORY_H_

#ifdef STUB
#  pragma @NoRename
#endif

#ifdef MEMFNS_IN_STRING_H
/*
 * Some systems have the mem* functions in string.h not
 * memory.h
 */

#  include <System/string.h>
#else
#  include <memory.h>
#endif

#ifdef NEED_GCC_PROTOTYPES
extern "C"
{
    void* memset(void*, int, unsigned long);
}
#endif

#endif
