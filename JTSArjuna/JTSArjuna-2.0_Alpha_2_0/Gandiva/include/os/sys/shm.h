/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: shm.h,v 1.4 1996/10/24 11:49:54 nmcl Exp $
 */

#ifndef OS_SYS_SHM_H_
#define OS_SYS_SHM_H_

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
