/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id$
 */

#include <System/iostream.h>

#ifndef ACCOUNT_H_
#  include "Account.h"
#endif

#include <Thread/Thread.h>
#include <Arjuna/AtomicA.h>

Account::Account (int& res)
		 : LockManager(ANDPERSISTENT)
{
    amount = 0;

    AtomicAction current;

    current.begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	current.commit();
	res = 0;
    }

    if (res != 0)
	current.abort();
}

Account::Account (int& res, const Uid& id)
		 : LockManager(id, ANDPERSISTENT)
{
    amount = 0;

    AtomicAction current;    

    current.begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	current.commit();
	res = 0;
    }

    if (res != 0)
	current.abort();
}   

Account::~Account ()
{
    terminate();
}

Account::Outcome Account::insert (int money)
{
    Outcome result = NOTDONE;
    AtomicAction current;    

    current.begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	amount += money;
	result = DONE;
    }
    else
	result = LOCKED;

    if (result == DONE)
    {
	current.commit();
    }
    else
	current.abort();

    return result;
}

Account::Outcome Account::withdraw (int money)
{
    Outcome result = NOTDONE;
    AtomicAction current;    

    current.begin();
    
    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	if (money <= amount)
	{
	    amount -= money;
	    result = DONE;
	}
	else
	{
	    cout << "Error - insufficient funds." << endl;
	    result = INSUFFICIENT_FUNDS;
	}
    }
    else
	result = LOCKED;

    if (result == DONE)
    {
	current.commit();
    }
    else
	current.abort();

    return result;
}
	
Account::Outcome Account::inspect (int& money)
{
    Account::Outcome result = NOTDONE;
    AtomicAction current;    

    current.begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	money = amount;
	result = DONE;
    }
    else
	result = LOCKED;

    if (result == DONE)
    {
	current.commit();
    }
    else
	current.abort();

    return result;
}

Boolean Account::save_state (ObjectState& os, ObjectType)
{
    return os.pack(amount);
}

Boolean Account::restore_state (ObjectState& os, ObjectType)
{
    return os.unpack(amount);
}

const TypeName Account::type () const
{
    return "/StateManager/LockManager/Account";
}
