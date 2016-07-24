/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: shm.h,v 1.1 1993/11/03 14:34:31 nmcl Exp $
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

#ifndef __linux__
        char *shmat(int, void*, int);
        int shmdt(const void*);
#else
	char *shmat(int, char*, int);
        int shmdt(char*);
#endif
        int shmctl(int, int, struct shmid_ds *);
}

#endif
#endif
