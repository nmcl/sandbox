/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: resource.h,v 1.1 1997/06/09 19:52:01 nmcl Exp $
 */

#ifndef SYS_RESOURCE_H_
#define SYS_RESOURCE_H_

#ifdef STUB
#  pragma @NoRename
#endif

#ifndef SYS_TIME_H_
#  include <System/sys/time.h>
#endif

#include <sys/resource.h>

#if defined(NEED_GCC_PROTOTYPES) || defined(__ultrix)
extern "C" {
    int getrlimit (int, rlimit*);
    int getrusage (int, struct rusage*);
}
#endif

#endif
