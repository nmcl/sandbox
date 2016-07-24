/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Manager.cc,v 1.2 1994/10/21 12:28:19 nsmw Exp $
 */

#include <System/iostream.h>

#ifndef MANAGER_H_
#  include <ObjectConfig/Manager.h>
#endif

#ifndef MULTIMAN_H_
#  include <ObjectConfig/MultiMan.h>
#endif

#ifndef CONFINDEX_H_
#  include <ObjectConfig/ConfIndex.h>
#endif

Manager::Manager (ConfigIndex *confIndex, MultiManager *multiMan)
{
    Uid newUid;

    configIndex  = confIndex;
    multiManager = multiMan;
    objectUid = newUid;

    if (!( configIndex->add(newUid, multiManager) && multiManager->do_add(objectUid, this, NULL)))
	objectUid = NIL_UID;
}

Manager::Manager (const Uid& uid, ConfigIndex *confIndex)
{
    configIndex  = confIndex;
    multiManager = NULL;
    objectUid    = uid;
}

Manager::~Manager ()
{
}

LockResult Manager::setlock (Lock *lock, int retry, unsigned int sleepTime)
{
    LockResult res;

    if (objectUid == NIL_UID)
	return REFUSED;

    if (multiManager == NULL)
    {
        if (! (configIndex->find(objectUid, multiManager) && multiManager->do_register(this)))
        {
            multiManager = NULL;
	    return REFUSED;
        }
    }

    if (! multiManager->do_setlock(this, res, lock, retry, sleepTime))
    {
        if (! (configIndex->find(objectUid, multiManager) && multiManager->do_register(this)))
        {
            multiManager = NULL;
	    return REFUSED;
        }
        else
        {
            if (! multiManager->do_setlock(this, res, lock, retry, sleepTime))
	        return REFUSED;
        }
    }

    return res;
}

Uid Manager::get_uid() const
{
    return objectUid;
}

void Manager::terminate()
{
}

Boolean Manager::save_state (ObjectState&, ObjectType)
{
    cerr << "Manager::save_state() res = FALSE" << endl;

    return FALSE;
}

Boolean Manager::restore_state (ObjectState&, ObjectType)
{
    cerr << "Manager::restore_state() res = FALSE" << endl;

    return FALSE;
}
