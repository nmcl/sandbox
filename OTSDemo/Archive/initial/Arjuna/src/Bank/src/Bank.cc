/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Bank.cc,v 1.5 1995/02/23 15:09:31 nmcl Exp $
 */

#ifndef BANK_H_
#  include "Bank.h"
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif


Bank::Bank (int& res)
	   : LockManager(ANDPERSISTENT)
{
    for (int i = 0; i < accountSize; i++)
	accounts[i] = 0;
    
    AtomicAction A;

    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
	res = ((A.End() == COMMITTED) ? 0 : -1);
    else
    {
	A.Abort();
	res = -1;
    }
}

Bank::Bank (int& res, const Uid& id)
	   : LockManager(id, ANDPERSISTENT)
{
     for (int i = 0; i < accountSize; i++)
	accounts[i] = 0;
    
    AtomicAction A;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
	res = ((A.End() == COMMITTED) ? 0 : -1);
    else
    {
	A.Abort();
	res = -1;
    }
}   

Bank::~Bank ()
{
    terminate();
}

Boolean Bank::insert (long number, long amount)
{
    if ((number < 0) || (number >= accountSize))
	return FALSE;
    
    AtomicAction A;
    Boolean result = FALSE;

    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	accounts[number] += amount;
	if (A.End() == COMMITTED)
	    result = TRUE;
    }
    else
	A.Abort();

    return result;
}

Boolean Bank::withdraw (long number, long amount)
{
    if ((number < 0) || (number >= accountSize))
	return FALSE;
    
    AtomicAction A;
    Boolean result = FALSE;

    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	accounts[number] -= amount;
	if (A.End() == COMMITTED)
	    result = TRUE;
    }
    else
	A.Abort();

    return result;
}

Boolean Bank::inspect (long number, long& amount)
{
    if ((number < 0) || (number >= accountSize))
	return FALSE;
    
    AtomicAction A;
    Boolean result = FALSE;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {    
	amount = accounts[number];
	if (A.End() == COMMITTED)
	    result = TRUE;
    }
    else
	A.Abort();

    return result;
}

Boolean Bank::save_state (ObjectState& os, ObjectType)
{
    Boolean res = TRUE;

    for (int i = 0; (i < accountSize) && (res); i++)
	res = res && os.pack(accounts[i]);

    return res;
}

Boolean Bank::restore_state (ObjectState& os, ObjectType)
{
    Boolean res = TRUE;

    for (int i = 0; (i < accountSize) && (res); i++)
	res = res && os.unpack(accounts[i]);

    return res;
}

const TypeName Bank::type () const
{
    return "/StateManager/LockManager/Bank";
}
