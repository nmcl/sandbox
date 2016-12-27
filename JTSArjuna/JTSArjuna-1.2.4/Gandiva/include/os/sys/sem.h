/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: sem.h,v 1.4 1996/01/02 13:13:45 nsmw Exp $
 */

#ifndef OS_SYS_SEM_H_
#define OS_SYS_SEM_H_

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

#ifndef LINUX
        int semop(int, struct sembuf *, int);
#else
        int semop(int, struct sembuf *, unsigned int);
#endif
}

#endif
#endif
