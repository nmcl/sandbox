/*
 * Copyright (C) 1995-1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id$
 */

#ifndef ACCOUNT_H_
#define ACCOUNT_H_

#ifndef LOCKMAN_H_
#  include <Arjuna/LockMan.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

class Account : public LockManager
{
public:
    enum Outcome { DONE, NOTDONE, UNKNOWN, INSUFFICIENT_FUNDS, ACCOUNT_ERROR, LOCKED };
    
    Account (int& res);
    Account (int&, const Uid& id);
    ~Account ();

    Outcome insert (int amount);
    Outcome withdraw (int amount);
    Outcome inspect (int& amount);

    virtual Boolean save_state (ObjectState& os, ObjectType);
    virtual Boolean restore_state (ObjectState& os, ObjectType);
    virtual const TypeName type () const;

private:
    int amount;
};

#endif





























