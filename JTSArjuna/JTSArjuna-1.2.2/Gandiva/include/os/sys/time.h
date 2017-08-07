/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: time.h,v 1.6 1996/01/08 12:14:59 nmcl Exp $
 */

#ifndef OS_SYS_TIME_H_
#define OS_SYS_TIME_H_

#include <sys/time.h>

#ifndef LINUX

#if defined(NEED_GCC_PROTOTYPES) || defined(__ultrix)
#ifndef SOLARIS2
extern "C" {
    int gettimeofday(struct timeval*, struct timezone*);
    int setitimer(int, /* const */ itimerval*, itimerval*);
}
#endif
#elif defined(SOLARIS2) && !defined(HAS_BSD_GETTIMEOFDAY)
extern "C" int gettimeofday(struct timeval*);
#endif

#endif

#endif
