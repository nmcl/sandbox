/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ActionInfo.java,v 1.6 1998/07/06 13:26:11 nmcl Exp $
 */

package com.arjuna.JavaArjunaLite.Atomic;

import com.arjuna.JavaGandiva.Common.Uid;

/*
 * Default visibility.
 */

class ActionInfo
{

public ActionInfo ()
    {
	actionUid = new Uid(Uid.nullUid());
	actionType = BasicAction.NESTED;
    }
    
public ActionInfo (ActionInfo other)
    {
	actionUid = new Uid(other.actionUid);
	actionType = other.actionType;
    }

public ActionInfo (Uid actionId, int at)
    {
	actionUid = new Uid(actionId);
	actionType = at;
    }    

public synchronized void copy (ActionInfo other)
    {
	if (this != other)
	{
	    actionUid = new Uid(other.actionUid);
	    actionType = other.actionType;
	}
    }
    
public final boolean equals (ActionInfo other)
    {
	if ((actionUid.equals(other.actionUid)) &&
	    (actionType == other.actionType))
	    return true;
	else
	    return false;
    }

public final boolean notEquals (ActionInfo other)
    {
	if ((actionUid.notEquals(other.actionUid)) ||
	    (actionType != other.actionType))
	    return true;
	else
	    return false;
    }

public Uid actionUid;
public int actionType;
    
}
