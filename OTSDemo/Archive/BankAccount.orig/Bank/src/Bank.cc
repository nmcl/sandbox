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

#include <System/iostream.h>

#ifndef BANK_H_
#  include "Bank.h"
#endif

#include <CosTransactions/OTS.h>
#include <CosTransactions/OTS_Current.h>


Bank::Bank (int& res)
	   : LockManager(ANDPERSISTENT)
{
    for (int i = 0; i < accountSize; i++)
    {
	accounts[i] = 0;
	pinNumbers[i] = 0;
    }

    pinNumbers[0] = 1234;
    pinNumbers[1] = 5678;

    OTS_Current& current = OTS::current();

    current.begin();

    EX_TRY
    {
	if (setlock(new Lock(WRITE), 0) == GRANTED)
	{
	    current.commit(0);
	    res = 0;
	}
    }
    EX_CATCHALL
    {
    }

    if (res != 0)
	current.rollback();
}

Bank::Bank (int& res, const Uid& id)
	   : LockManager(id, ANDPERSISTENT)
{
    for (int i = 0; i < accountSize; i++)
	accounts[i] = 0;

    OTS_Current& current = OTS::current();

    current.begin();

    EX_TRY
    {     
	if (setlock(new Lock(WRITE), 0) == GRANTED)
	{
	    current.commit(0);
	    res = 0;
	}
    }
    EX_CATCHALL
    {
    }

    if (res != 0)
	current.rollback();
}   

Bank::~Bank ()
{
    terminate();
}

int Bank::getNumber (int number)
{
    int n = -1;
    OTS_Current& current = OTS::current();

    cout << "Checking pin validity." << endl;

    current.begin();
    
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	Boolean found = FALSE;
	
	for (int i = 0; (i < accountSize) && (!found); i++)
	{
	    if (pinNumbers[i] == number)
	    {
		n = i;
		found = TRUE;
	    }
	}
    }

    if (n == -1)
	cout << "Error - pin is invalid." << endl;
    else
	cout << "Pin is valid." << endl;
    
    current.commit(0);

    return n;
}
	    
Bank::Outcome Bank::insert (int number, int amount)
{
    Outcome result = NOTDONE;
    OTS_Current& current = OTS::current();

    current.begin();

    int account = getNumber(number);

    if (account != -1)
    {
	if (setlock(new Lock(WRITE), 0) == GRANTED)
	{
	    accounts[account] += amount;
	    result = DONE;
	}
    }
    else
	result = ACCOUNT_ERROR;

    if (result == DONE)
    {
	current.commit(0);
    }
    else
	current.rollback();

    return result;
}

Bank::Outcome Bank::withdraw (int number, int amount)
{
    Outcome result = NOTDONE;
    OTS_Current& current = OTS::current();

    current.begin();
    
    int account = getNumber(number);

    if (account != -1)
    {
	if (setlock(new Lock(WRITE), 0) == GRANTED)
	{
	    if (amount <= accounts[account])
	    {
		accounts[account] -= amount;
		result = DONE;
	    }
	    else
	    {
		cout << "Error - insufficient funds." << endl;
		result = INSUFFICIENT_FUNDS;
	    }
	}
    }
    else
	result = ACCOUNT_ERROR;

    if (result == DONE)
    {
	current.commit(0);
    }
    else
	current.rollback();

    return result;
}

Bank::Outcome Bank::transfer (int pinFrom, int pinTo, int amount)
{
    Bank::Outcome result = NOTDONE;
    OTS_Current& current = OTS::current();

    current.begin();

    result = withdraw(pinFrom, amount);

    if (result == DONE)
	result = insert(pinTo, amount);

    if (result == DONE)
    {
	current.commit(0);
    }
    else
	current.rollback();

    return result;
}
	
Bank::Outcome Bank::inspect (int number, int& amount)
{
    Bank::Outcome result = NOTDONE;
    OTS_Current& current = OTS::current();

    current.begin();
    
    int account = getNumber(number);

    if (account != -1)
    {
	if (setlock(new Lock(READ), 0) == GRANTED)
	{    
	    amount = accounts[account];
	    result = DONE;
	}
    }
    else
	result = ACCOUNT_ERROR;

    if (result == DONE)
    {
	current.commit(0);
    }
    else
	current.rollback();

    return result;
}

Boolean Bank::save_state (ObjectState& os, ObjectType)
{
    Boolean res = TRUE;

    for (int i = 0; (i < accountSize) && (res); i++)
	res = res && os.pack(accounts[i]) && os.pack(pinNumbers[i]);

    return res;
}

Boolean Bank::restore_state (ObjectState& os, ObjectType)
{
    Boolean res = TRUE;

    for (int i = 0; (i < accountSize) && (res); i++)
	res = res && os.unpack(accounts[i]) && os.unpack(pinNumbers[i]);

    return res;
}

const TypeName Bank::type () const
{
    return "/StateManager/LockManager/Bank";
}
