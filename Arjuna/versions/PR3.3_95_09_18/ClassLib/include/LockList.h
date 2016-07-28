/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LockList.h,v 1.3 1995/03/13 15:02:15 ngdp Exp $
 */

#ifndef LOCKLIST_H_
#define LOCKLIST_H_

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

/*
 *
 * Lock list maintainance class interface
 *
 */

class Lock;

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
