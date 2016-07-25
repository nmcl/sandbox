/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LockList.h,v 1.4 1993/03/22 09:16:04 ngdp Exp $
 */

#ifndef LOCKLIST_H_
#define LOCKLIST_H_

/*
 *
 * Lock list maintainance class interface
 *
 */

#ifndef LOCK_H_
#  include <Arjuna/Lock.h>
#endif

class LockList
{
    friend class LockListI;	/* allow access to private members */

public:
    /* Constructors and destructor */
    LockList ();
    ~LockList ();

    /* non-virtual member functions and operators */

    int entryCount () const;
    Boolean insert (Lock *);
    Lock *pop ();
    void push (Lock *newLock);
    void forgetNext (Lock *current);

private:
    /* private state variables */

    int count;				/* current number of members */
    Lock *head;				/* head of list */
    
};

#include "LockList.n"

#endif
