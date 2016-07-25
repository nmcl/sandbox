/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Semaphore.h,v 1.1 1993/11/03 12:29:16 nmcl Exp $
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
#  include <System/sys/types.h>
#endif

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

class Semaphore
{
public:
    /* Constructors and destructor */

    Semaphore (const key_t semK);
    Semaphore ();
    ~Semaphore ();
    
    /* classic semaphore operations */

    void signal ();
    void wait ();		      

private:
    /* non-virtual member function */

    Boolean semCreate ();
    
    /* private state variables */

    key_t semKey;
    int semId;				/* the id as known by the system */
};

#endif
