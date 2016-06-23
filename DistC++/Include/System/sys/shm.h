/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: shm.h,v 1.1 1997/09/25 15:27:41 nmcl Exp $
 */

#ifndef SYS_SHM_H_
#define SYS_SHM_H_

#ifdef STUB
#  pragma @NoRename
#endif

#include <sys/shm.h>

#if defined(NEED_GCC_PROTOTYPES)

extern "C" {
        int shmget(key_t, int, int);
        char *shmat(int, void*, int);
        int shmdt(const void*);
        int shmctl(int, int, struct shmid_ds *);
}

#endif
#endif
