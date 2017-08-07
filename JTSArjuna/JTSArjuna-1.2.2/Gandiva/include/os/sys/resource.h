/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: resource.h,v 1.4 1996/10/24 11:49:53 nmcl Exp $
 */

#ifndef OS_SYS_RESOURCE_H_
#define OS_SYS_RESOURCE_H_

#ifndef OS_SYS_TIME_H_
#  include <os/sys/time.h>
#endif

#include <sys/resource.h>

#if defined(NEED_GCC_PROTOTYPES) || defined(__ultrix)
extern "C"
{
    int getrlimit (int, rlimit*);
    int getrusage (int, struct rusage*);
}
#endif

#endif
