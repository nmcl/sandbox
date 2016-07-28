/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RecArray.cc,v 1.13 1995/07/03 14:30:11 ngdp Exp $
 */


#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
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

#ifndef COMPLEXOBJECT_H_
#  include <Replication/COControl.h>
#endif

#ifndef REPLICACREATE_H_
#  include <Replication/ReplicaCreate.h>
#endif

#ifndef REGISTERGROUP_H_
#  include <Replication/RegisterGroup.h>
#endif

#ifndef RECARRAY_H_
#  include "RecArray.h"
#endif


const int numberReplicas = 2;


RecArray::RecArray (int& res) : LockManager(ANDPERSISTENT)
{
    AtomicAction A;
    Boolean ok = TRUE;
    char* replicas[numberReplicas];
    int i = 0;

    res = -1;

    /* Must be done before object store is created, i.e., before setlock. */

    for (i = 0; i < numberReplicas; i++)
    {
	replicas[i] = ::new char[MAXHOSTNAMELEN+1];
	::memset(replicas[i], '\0', MAXHOSTNAMELEN+1);
	(void)gethostname(replicas[i], MAXHOSTNAMELEN);
    }

    StateManager::setStoreInformation(replicas, numberReplicas);

    for (i = 0; (i < SIZE) && (ok); i++)
    {
	int ret;

	ret = -1;
	rArray[i] = new RecInt(ret);
	if (ret != 0)
	    ok = FALSE;
    }

    if (ok)
    {
	A.Begin();

	if (setlock(new Lock(WRITE), 0) == GRANTED)
	{
	    if (A.End() == COMMITTED)
		res = 0;
	}
	else
	    A.Abort();
    }
    else
	cout << "RecInt constructor error" << endl;

    for (i = 0; i < numberReplicas; i++)
#ifndef GCC_ARRAY_DELETE_BUG
	::delete [] replicas[i];
#else
        ::delete replicas[i];
#endif    
}

RecArray::RecArray (const Uid& u, int& res) : LockManager(u)
{
    AtomicAction A;
    
    for (int j = 0; j < SIZE; j++)
        rArray[j] = 0;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (A.End() == COMMITTED)
	    res = 0;
    }
    else
	A.Abort();
}

RecArray::~RecArray ()
{
    terminate();
    for (int i = 0; i < SIZE; i++)
    {
        if (rArray[i])
	    delete rArray[i];
    }
}

Boolean RecArray::hasRemoteState () const { return TRUE; }
        
Boolean RecArray::setElement (int index, int value)
{
    AtomicAction A;
    Boolean result = FALSE;

    if ((index >= SIZE) || (index < 0))
    {
	cout << "Array bounds error. Number of elements is: " << SIZE << endl;
	return FALSE;
    }

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
        result = rArray[index]->change(value);

    if (result)
    {
        if (A.End() != COMMITTED)
	    result = FALSE;
    }
    else
        A.Abort();

    return result;
}

int RecArray::getElement (int index)
{
    AtomicAction A;
    int value = -1;

    if ((index >= SIZE) || (index < 0))
    {
	cout << "Array bounds error. Number of elements is: " << SIZE << endl;
	return FALSE;
    }

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
        value = rArray[index]->value();

    if (value >= 0)
    {
        if (A.End() != COMMITTED)
	    value = -1;
    }
    else
        A.Abort();

    return value;
}

/*
 * These routines must operate before the state of the envelope object is
 * actually used because they tell the object how it is replicated and which
 * other objects it is associated with. If these operations are attempted
 * within an action which is using the object (i.e., the object has been
 * activated) then the reconfiguration will fail.
 * The state of the envelope object needs to have been set-up prior to this
 * action to prevent any conflicts with the replication top-level actions.
 * This *must* be done in a separate action, and in this example we do this
 * by activating the object in the constructor.
 */

Boolean RecArray::associate ()
{
    Boolean result = TRUE;
    ComplexObjectControl cO;
    AtomicAction A;

    A.Begin();

    for (int i = 0; (i < SIZE) && (result); i++)
	result = (cO.associateGroups(*this, *rArray[i]) == DONE);

    if (result)
    {
	if (A.End() != COMMITTED)
	    result = FALSE;
    }
    else
    {
	cout << "associate error." << endl;
	A.Abort();
    }
    
    return result;
}

Boolean RecArray::disassociate ()
{
    Boolean result = TRUE;
    ComplexObjectControl cO;
    AtomicAction A;

    A.Begin();

    for (int i = 0; (i < SIZE) && (result); i++)
	result = (cO.disassociateGroups(*this, *rArray[i]) == DONE);

    if (result)
    {
	if (A.End() != COMMITTED)
	    result = FALSE;
    }
    else
    {
	cout << "disassociate error." << endl;
	A.Abort();
    }
    
    return result;
}

Boolean RecArray::save_state (ObjectState& os, ObjectType)
{
    Boolean result = TRUE;
    
    for (int i = 0; (i < SIZE) && (result); i++)
	result = (rArray[i]->get_uid()).pack(os);

    return result;
}

Boolean RecArray::restore_state (ObjectState& os, ObjectType)
{
    Boolean result = TRUE;

    for (int i = 0; (i < SIZE) && (result); i++)
    {
	Uid* u = new Uid;
	result = u->unpack(os);

	if (rArray[i] == 0)
	{
	    if (result)
	    {
		int res = -1;
		rArray[i] = new RecInt(*u, res);
		if (res != 0)
		    result = FALSE;
	    }
	}

	delete u;
    }

    return result;
}

const TypeName RecArray::type () const
{
    return "/StateManager/LockManager/RecArray";
}
