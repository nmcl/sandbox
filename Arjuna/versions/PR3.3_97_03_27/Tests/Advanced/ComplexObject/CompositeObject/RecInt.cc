/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RecInt.cc,v 1.7 1995/07/03 14:30:13 ngdp Exp $
 */


#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef NETDB_H_
#   include <System/netdb.h>
#endif

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef RECINT_H_
#  include "RecInt.h"
#endif


const int numberOfReplicas = 2;


RecInt::RecInt (int& res) : LockManager(ANDPERSISTENT)
{
    AtomicAction A;
    char* replicas[numberOfReplicas];
    int i;
    
    
    /* Must be done before object store is created, i.e., before setlock. */

    for (i = 0; i < numberOfReplicas; i++)
    {
	replicas[i] = ::new char[MAXHOSTNAMELEN+1];
	::memset(replicas[i], '\0', MAXHOSTNAMELEN+1);
	(void)gethostname(replicas[i], MAXHOSTNAMELEN);
    }

    StateManager::setStoreInformation(replicas, numberOfReplicas);

    A.Begin();

    number = 0;
    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
        if (A.End() == COMMITTED)
	    res = 0;
    }
    else
        A.Abort();
    
    for (i = 0; i < numberOfReplicas; i++)
#ifndef GCC_ARRAY_DELETE_BUG
	::delete [] replicas[i];
#else
        ::delete replicas[i];
#endif    
}

RecInt::RecInt (const Uid& u, int& res) : LockManager(u)
{
    res = 0;
}

RecInt::~RecInt () { terminate(); }

Boolean RecInt::hasRemoteState () const { return TRUE; }

Boolean RecInt::change (int value)
{
    AtomicAction A;

    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
        number = value;
	if (A.End() == COMMITTED)
	    return TRUE;
    }
    else
        A.Abort();

    return FALSE;
}

int RecInt::value ()
{
    AtomicAction A;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
        if (A.End() == COMMITTED)
	    return number;
    }
    else
        A.Abort();

    return -1;
}

Boolean RecInt::save_state (ObjectState& os, ObjectType)
{
    return os.pack(number);
}

Boolean RecInt::restore_state (ObjectState& os, ObjectType)
{
    return os.unpack(number);
}

const TypeName RecInt::type () const
{
    return "/StateManager/LockManager/RecInt";
}
