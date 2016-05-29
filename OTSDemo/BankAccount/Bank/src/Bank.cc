/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Bank.cc,v 1.5 1995/02/23 15:09:31 nmcl Exp $
 */

#include <System/iostream.h>

#ifndef BANK_H_
#  include "Bank.h"
#endif

#ifndef ACCOUNT_H_
#  include "Account.h"
#endif

#include <Thread/Thread.h>
#include <Arjuna/AtomicA.h>

Bank::Bank (int& res)
	   : LockManager(ANDPERSISTENT)
{
    for (int i = 0; i < accountSize; i++)
    {
	accounts[i] = new Account(res);
	pinNumbers[i] = 0;
    }

    pinNumbers[0] = 1234;
    pinNumbers[1] = 5678;

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

Bank::Bank (int& res, const Uid& id)
	   : LockManager(id, ANDPERSISTENT)
{
    for (int i = 0; i < accountSize; i++)
	accounts[i] = 0;

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

Bank::~Bank ()
{
    for (int i = 0; i < accountSize; i++)
    {
	if (accounts[i])
	    delete accounts[i];
    }
    
    terminate();
}

int Bank::getNumber (int number)
{
    int n = -1;
    AtomicAction current;
    Boolean found = FALSE;

    cout << "Checking pin validity." << endl;

    for (int i = 0; (i < accountSize) && (!found); i++)
    {
	if (pinNumbers[i] == number)
	{
	    n = i;
	    found = TRUE;
	}
    }

    if (n == -1)
    {
	cout << "Error - pin is invalid." << endl;
    }
    else
    {
	cout << "Pin is valid." << endl;
    }

    return n;
}
	    
Bank::Outcome Bank::insert (int number, int amount)
{
    Outcome result = NOTDONE;
    AtomicAction current;    

    current.begin();

    int account = getNumber(number);

    if (account != -1)
    {
	if (setlock(new Lock(READ), 0) == GRANTED)
	{
	    result = (Bank::Outcome) accounts[account]->insert(amount);
	    
	    if (number == pinNumbers[1])
		Thread::sleep(sleepTime);
	}
	else
	    result = LOCKED;
    }
    else
	result = ACCOUNT_ERROR;

    if (result == DONE)
    {
	current.commit();
    }
    else
	current.abort();

    return result;
}

Bank::Outcome Bank::withdraw (int number, int amount)
{
    Outcome result = NOTDONE;
    AtomicAction current;    

    current.begin();
    
    int account = getNumber(number);

    if (account != -1)
    {
	if (setlock(new Lock(READ), 0) == GRANTED)
	{
	    result = (Bank::Outcome) accounts[account]->withdraw(amount);
	}
	else
	    result = LOCKED;
    }
    else
	result = ACCOUNT_ERROR;

    if (result == DONE)
    {
	current.commit();
    }
    else
	current.abort();

    return result;
}

Bank::Outcome Bank::transfer (int pinFrom, int pinTo, int amount)
{
    Bank::Outcome result = NOTDONE;
    AtomicAction current;    

    current.begin();

    result = withdraw(pinFrom, amount);

    if (result == DONE)
	result = insert(pinTo, amount);

    if (result == DONE)
    {
	current.commit();
    }
    else
	current.abort();

    return result;
}
	
Bank::Outcome Bank::inspect (int number, int& amount)
{
    Bank::Outcome result = NOTDONE;
    AtomicAction current;    

    current.begin();
    
    int account = getNumber(number);

    if (account != -1)
    {
	if (setlock(new Lock(READ), 0) == GRANTED)
	{
	    result = (Bank::Outcome) accounts[account]->inspect(amount);
	}
	else
	    result = LOCKED;
    }
    else
	result = ACCOUNT_ERROR;

    if (result == DONE)
    {
	current.commit();
    }
    else
	current.abort();

    return result;
}

Boolean Bank::save_state (ObjectState& os, ObjectType)
{
    Boolean res = TRUE;

    for (int i = 0; (i < accountSize) && (res); i++)
	res = res && accounts[i]->get_uid().pack(os) && os.pack(pinNumbers[i]);

    return res;
}

Boolean Bank::restore_state (ObjectState& os, ObjectType)
{
    Boolean res = TRUE;

    for (int i = 0; (i < accountSize) && (res); i++)
    {
	if (accounts[i] == 0)
	{
	    Uid u(NIL_UID);

	    if (u.unpack(os))
	    {
		int v = -1;
	    
		accounts[i] = new Account(v, u);

		if (v == 0)
		    res = os.unpack(pinNumbers[i]);
		else
		    res = FALSE;
	    }
	    else
		res = FALSE;
	}
	else
	    res = TRUE;
    }

    return res;
}

const TypeName Bank::type () const
{
    return "/StateManager/LockManager/Bank";
}
