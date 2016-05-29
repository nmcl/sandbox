/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Bank.h,v 1.5 1995/02/23 15:09:31 nmcl Exp $
 */

#ifndef BANK_H_
#define BANK_H_

#ifndef LOCKMAN_H_
#  include <Arjuna/LockMan.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

const int accountSize = 10;
const unsigned int sleepTime = 10000000;

class Account;

class Bank : public LockManager
{
public:
    enum Outcome { DONE, NOTDONE, UNKNOWN, INSUFFICIENT_FUNDS, ACCOUNT_ERROR, LOCKED };
    
    Bank (int& res);
    Bank (int&, const Uid& id);
    ~Bank ();

    Outcome insert (int number, int amount);
    Outcome withdraw (int number, int amount);
    Outcome inspect (int number, int& amount);
    Outcome transfer (int fromPin, int toPin, int amount);

    virtual Boolean save_state (ObjectState& os, ObjectType);
    virtual Boolean restore_state (ObjectState& os, ObjectType);
    virtual const TypeName type () const;

private:
    int getNumber (int number);
    
    Account* accounts[accountSize];
    int pinNumbers[accountSize];
};

#endif





























