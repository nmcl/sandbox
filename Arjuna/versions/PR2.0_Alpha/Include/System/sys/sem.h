/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: sem.h,v 1.6 1993/05/05 13:40:43 ngdp Exp $
 */

#ifndef SYS_SEM_H_
#define SYS_SEM_H_

#ifdef STUB
#  pragma @NoRename
#endif

#include <sys/sem.h>

#ifdef SYSV

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

#if defined(__GNUG__) && !defined(SYSV)

extern "C" {
        int semget(key_t, int, int);
        int semctl(int, int, int, union semun);
        int semop(int, struct sembuf *, int);
}

#endif
#endif
