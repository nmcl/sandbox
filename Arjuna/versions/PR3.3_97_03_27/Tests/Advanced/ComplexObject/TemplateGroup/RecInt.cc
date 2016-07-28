/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RecInt.cc,v 1.3 1994/10/14 11:03:49 nmcl Exp $
 */


#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
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

#ifndef RECINT_H_
#  include "RecInt.h"
#endif

extern ComplexObjectControl controller;


/*
 * Because this is a template object we use this constructor to obtain
 * the Uid for adding to the template entry at the database. Since this
 * must be done before the object is actually "used", we do this before
 * we start the top-level action which will save the state of the object.
 */

RecInt::RecInt (int& res, const Uid& templateUid) : LockManager(ANDPERSISTENT)
{
    AtomicAction A;
    
    number = -1;
    res = -1;

    controller.groupWithTemplate(templateUid, *this, TRUE);

    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	if (A.End() == COMMITTED)
	    res = 0;
    }
    else
	A.Abort();
}

RecInt::RecInt (const Uid& u, int& res) : LockManager(u)
{
    number = -1;    
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
