/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RecArray.cc,v 1.12 1995/07/03 14:30:14 ngdp Exp $
 */


#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef NETDB_H_
#  include <System/netdb.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef COMPLEXOBJECT_H_
#  include <Replication/COControl.h>
#endif

#ifndef RECARRAY_H_
#  include "RecArray.h"
#endif

extern ComplexObjectControl controller;


/*
 * Because this is a template object we use this constructor to obtain
 * the Uid for adding to the template entry at the database. Since this
 * must be done before the object is actually "used", we do this before
 * we start the top-level action which will save the state of the object.
 * Once this has been registered we create all of the sub-objects and get
 * them to register themselves (within top-level actions).
 */

RecArray::RecArray (int& res) : LockManager(ANDPERSISTENT)
{
    char* replicas[numberReplicas];
    AtomicAction A;
    int ret;
    int i;
    
    res = -1;
    initialize();

    /* Register basic template at database. */

    for (i = 0; i < numberReplicas; i++)
    {
	replicas[i] = ::new char[MAXHOSTNAMELEN+1];
	::memset(replicas[i], '\0', MAXHOSTNAMELEN+1);
	(void)gethostname(replicas[i], MAXHOSTNAMELEN);
    }

    controller.addTemplate(templateUid, replicas, numberReplicas);
    controller.groupWithTemplate(templateUid, *this, TRUE);

    ret = 0;
    for (i = 0; (i < SIZE) && (ret == 0); i++)
    {
	ret = -1;
	rArray[i] = new RecInt(ret, templateUid);
    }

    if (ret == 0)
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

    initialize();

    res = -1;
    
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

void RecArray::initialize ()
{
    for (int j = 0; j < SIZE; j++)
        rArray[j] = 0;
}

Boolean RecArray::hasRemoteState () const { return TRUE; }
        
Boolean RecArray::setElement (int index, int value)
{
    AtomicAction A;
    Boolean result = FALSE;

    if ((index >= SIZE) || (index < 0))
	return FALSE;

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
	return FALSE;

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
 * These routines must be called prior to the use of any of the
 * object states mentioned in the grouping.
 */

Boolean RecArray::associate ()
{
    Boolean result = TRUE;
    AtomicAction A;

    A.Begin();

    result = (controller.groupWithTemplate(templateUid, *this) == DONE);
    for (int i = 0; (i < SIZE) && (result); i++)
	result = (controller.groupWithTemplate(templateUid, *rArray[i]) == DONE);

    if (result)
    {
        if (A.End() != COMMITTED)
	    result = FALSE;
    }
    else
        A.Abort();

    return result;
}

Boolean RecArray::disassociate ()
{
    Boolean result = TRUE;
    AtomicAction A;

    A.Begin();

    result = (controller.ungroupFromTemplate(templateUid, *this) == DONE);
    
    for (int i = 0; (i < SIZE) && (result); i++)
	result = (controller.ungroupFromTemplate(templateUid, *rArray[i]) == DONE);

    if (result)
    {
        if (A.End() != COMMITTED)
	    result = FALSE;
    }
    else
        A.Abort();

    return result;
}
    
Boolean RecArray::save_state (ObjectState& os, ObjectType)
{
    Boolean result = templateUid.pack(os);

    for (int i = 0; (i < SIZE) && (result); i++)
	result = (rArray[i]->get_uid()).pack(os);

    return result;
}

Boolean RecArray::restore_state (ObjectState& os, ObjectType)
{
    Boolean result = templateUid.unpack(os);

    for (int i = 0; (i < SIZE) && (result); i++)
    {
	if (rArray[i] == 0)
	{
	    Uid* u = new Uid;
	    result = u->unpack(os);
	    if (result)
	    {
		int res = -1;
		rArray[i] = new RecInt(*u, res);
		if (res != 0)
		    result = FALSE;
	    }

	    delete u;
	}
    }

    return result;
}

const TypeName RecArray::type () const
{
    return "/StateManager/LockManager/RecArray";
}
