/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LockListI.h,v 1.3 1993/03/22 09:16:05 ngdp Exp $
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

#include "LockListI.n"

#endif
