/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Replica.h,v 1.1 1995/09/18 11:36:58 nmcl Exp $
 */

#ifndef REPLICA_H_
#define REPLICA_H_

#include <System/sys/param.h>
#include <System/netdb.h>
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

    Boolean hasRemoteState () const;

    virtual Boolean save_state (ObjectState&, ObjectType);
    virtual Boolean restore_state (ObjectState&, ObjectType);
    virtual const TypeName type () const;

private:
    int number;
    char hostName[MAXHOSTNAMELEN+1];
};

#endif

