/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Replica.cc,v 1.1 1995/09/18 11:36:57 nmcl Exp $
 */


#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef REPLICA_H_
#  include "Replica.h"
#endif


const int numberReplicas = 2;


Replica::Replica(int& res) : LockManager(ANDPERSISTENT)
{
    AtomicAction A;
    char* replicas[numberReplicas];

    A.Begin();

    number = 0;
    ::memset(hostName, '\0', MAXHOSTNAMELEN+1);
    (void)gethostname(hostName, MAXHOSTNAMELEN);

    for (int i = 0; i < numberReplicas; i++)
	replicas[i] = hostName;

    StateManager::setStoreInformation(replicas, numberReplicas);

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	if (A.End() == COMMITTED)
	    res = 0;
    }
    else
	A.Abort();
}

Replica::Replica(Uid& u, int& res) : LockManager(u)
{
    res = 0;
}

Replica::~Replica() { terminate(); }

Boolean Replica::change(int new_value)
{
    AtomicAction B;

    B.Begin();
    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	number = new_value;
	B.End();
	return TRUE;
    }
    else
    {
	B.Abort();
	return FALSE;
    }
}

int Replica::value()
{
    AtomicAction C;

    C.Begin();
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	C.End();
	return number;
    }
    else
    {
	C.Abort();
	return -1;
    }
}

Boolean Replica::hasRemoteState () const { return TRUE; }

Boolean Replica::save_state(ObjectState& os, ObjectType)
{
    return os.pack(number);
}

Boolean Replica::restore_state(ObjectState& os, ObjectType)
{
    return os.unpack(number);
}

const TypeName Replica::type() const
{
    return "/StateManager/LockManager/Replica";
}

