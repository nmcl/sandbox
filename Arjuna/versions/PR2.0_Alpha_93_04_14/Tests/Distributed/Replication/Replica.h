/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Replica.h,v 1.2 1993/03/18 15:07:00 n048z Exp $
 */

#ifndef REPLICA_H_
#define REPLICA_H_

#include <Common/Boolean.h>
#include <Arjuna/LockMan.h>
#include <Arjuna/ObjState.h>

class Replica : public LockManager
{
public:
    Replica (int&);
    Replica (Uid&, int&);
    ~Replica ();

    Boolean change(int);
    int value();

    void makeReplicated (Boolean);

    Boolean HasRemoteState () const;

    virtual Boolean save_state (ObjectState&, ObjectType);
    virtual Boolean restore_state (ObjectState&, ObjectType);
    virtual const TypeName type () const;

private:
    int number;
    Boolean Replicated;
};

#endif

