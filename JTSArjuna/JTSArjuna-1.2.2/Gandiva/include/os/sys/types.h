/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: types.h,v 1.7 1998/01/12 13:04:14 nmcl Exp $
 */

#ifndef OS_SYS_TYPES_H_
#define OS_SYS_TYPES_H_ 

#include <sys/types.h>

#ifdef NEED_ANY_T
#  ifndef any_t
#    define any_t void*
#  endif
#endif

#ifdef WIN32

#  ifndef OS_WINDOWS_H_
#    include <os/windows.h>
#  endif

#  define key_t HANDLE

#endif

#endif
