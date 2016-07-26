/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Semaphore.h,v 1.2 1995/08/16 13:58:38 ngdp Exp $
 */

#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

/*
 *
 * Semaphore class interface description. Provide simple interface to
 * System V semaphore handling routines.
 *
 */

#ifndef SYS_TYPES_H_
#  include <os/sys/types.h>
#endif

#ifndef SYS_IPC_H_
#  include <os/sys/ipc.h>
#endif

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

class Semaphore
{
public:
    enum Status { SEM_LOCKED, SEM_UNLOCKED, SEM_BLOCKED, SEM_ERROR };
    
    
    /* Constructors and destructor */

    Semaphore (const key_t, unsigned int = 1);
    ~Semaphore ();
    
    /* classic semaphore operations */

    Status signal ();
    Status trywait ();
    Status wait ();		      

private:
    /* non-virtual member function */

    Boolean semCreate ();
    
    /* private state variables */

    unsigned int initVal;
    key_t semKey;
    int semId;				/* the id as known by the system */
};

#endif
