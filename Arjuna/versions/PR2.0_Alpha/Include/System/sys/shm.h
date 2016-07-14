/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: shm.h,v 1.5 1993/05/05 13:40:46 ngdp Exp $
 */

#ifndef SYS_SHM_H_
#define SYS_SHM_H_

#ifdef STUB
#  pragma @NoRename
#endif

#include <sys/shm.h>

#if defined(__GNUG__) && !defined(SYSV)

extern "C" {
        int shmget(key_t, int, int);
        char *shmat(int, void*, int);
        int shmctl(int, int, struct shmid_ds *);
        int shmdt(const void*);
}

#endif
#endif
