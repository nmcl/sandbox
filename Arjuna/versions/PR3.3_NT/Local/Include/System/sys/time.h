/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: time.h,v 1.8 1996/04/15 14:17:47 nmcl Exp $
 */

#ifndef SYS_TIME_H_
#define SYS_TIME_H_

#ifdef STUB
#  pragma @NoRename
#endif

// @NoRemote
struct timeval;

#ifndef WIN32

#include <sys/time.h>

#if defined(NEED_GCC_PROTOTYPES) || defined(__ultrix)
extern "C" {
    int gettimeofday(struct timeval*, struct timezone*);
    int setitimer(int, /* const */ itimerval*, itimerval*);
}
#endif

#else

#include <time.h>

#endif

#endif
