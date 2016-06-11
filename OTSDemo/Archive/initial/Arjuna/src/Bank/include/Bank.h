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


class Bank : public LockManager
{
public:
    Bank (int& res);
    Bank (int&, const Uid& id);
    ~Bank ();

    Boolean insert (long number, long amount);
    Boolean withdraw (long number, long amount);
    Boolean inspect (long number, long& amount);

    virtual Boolean save_state (ObjectState& os, ObjectType);
    virtual Boolean restore_state (ObjectState& os, ObjectType);
    virtual const TypeName type () const;

private:
    long accounts[accountSize];
};

#endif





























