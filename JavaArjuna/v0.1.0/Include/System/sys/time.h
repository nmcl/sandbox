/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: time.h,v 1.7 1995/03/08 10:03:57 ngdp Exp $
 */

#ifndef SYS_TIME_H_
#define SYS_TIME_H_

#ifdef STUB
#  pragma @NoRename
#endif

// @NoRemote
struct timeval;

#include <sys/time.h>

#if defined(NEED_GCC_PROTOTYPES) || defined(__ultrix)
extern "C" {
    int gettimeofday(struct timeval*, struct timezone*);
    int setitimer(int, /* const */ itimerval*, itimerval*);
}
#endif

#endif
