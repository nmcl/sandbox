/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: sem.h,v 1.9 1995/08/16 13:52:26 ngdp Exp $
 */

#ifndef SYS_SEM_H_
#define SYS_SEM_H_

#ifdef STUB
#  pragma @NoRename
#endif

#ifdef __ultrix
/*
 * ULTRIX appears to have broken definitions for some semaphore operations
 */

#  define semop semop__
#  define semctl semctl__
#endif

#include <sys/sem.h>

#if defined(SYSV) || defined(__osf__) || defined(__ultrix)
#  define NEED_UNION_SEMUN
#endif
  
#ifdef __LINUX__
#  undef NEED_UNION_SEMUN
#endif

#ifdef NEED_UNION_SEMUN

/* 
 * This union does not appear to be defined in SunOS5 at least despite its
 * appearance on the man pages! Taken directly from there
 */

union semun
{
    int val;
    struct semid_ds* buf;
    ushort *array;
};

#endif

#ifdef __ultrix
#  undef semop
#  undef semctl
#endif

#if defined(NEED_GCC_PROTOTYPES) || defined(__ultrix)

extern "C" {
        int semget(key_t, int, int);
        int semctl(int, int, int, union semun);
        int semop(int, struct sembuf *, int);
}

#endif
#endif
