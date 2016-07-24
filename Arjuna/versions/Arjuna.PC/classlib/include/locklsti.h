/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LockListI.h,v 1.1 1993/11/03 12:28:55 nmcl Exp $
 */

#ifndef LOCKLISTI_H_
#define LOCKLISTI_H_

/*
 *
 * Lock iterator class
 *
 */

class Lock;
class LockList;

class LockListI
{
public:
    /* Constructors and destructor */

    LockListI (const LockList& L);
    ~LockListI ();

    /* non-virtual functions and operators */

    void reset ();
    Lock *operator() ();

private:
    /* private state variables */
    
    const LockList& currentList;	/* list we are curently walking */
    Lock *next;				/* return this one next time */
};

#include "LockLstI.n"

#endif
