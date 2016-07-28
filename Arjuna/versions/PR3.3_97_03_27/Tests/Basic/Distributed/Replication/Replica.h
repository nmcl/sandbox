/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Replica.h,v 1.3 1994/01/10 09:50:48 ngdp Exp $
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

