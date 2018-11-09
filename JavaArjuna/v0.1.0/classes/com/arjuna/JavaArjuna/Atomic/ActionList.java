/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ActionList.java,v 1.5 1998/07/06 13:26:12 nmcl Exp $
 */

package com.arjuna.JavaArjunaLite.Atomic;

public class ActionList
{

public ActionList ()
    {
	_actionList = null;
	_number = 0;
    }

public final synchronized boolean present (BasicAction u)
    {
	if (_number == 0)
	    return false;
	
	ActionElement ptr = _actionList;
	
	while (ptr != null)
	{
	    if (ptr.equals(u))
		return true;
	    else
		ptr = ptr.next();
	}

	return false;
    }
    
public final synchronized boolean insert (BasicAction u)
    {
	if (_actionList == null)
	    _actionList = new ActionElement(u);
	else
	{
	    ActionElement ptr = _actionList;

	    while (ptr != null)
	    {
		if (ptr.equals(u))
		    return false;
		else
		    ptr = ptr.next();
	    }

	    /*
	     * This must be a new action.
	     */

	    ptr = new ActionElement(u, _actionList);
	    _actionList = ptr;
	}

	_number++;

	return true;
    }

public final synchronized boolean remove (BasicAction u)
    {
	if (_actionList != null)
	{
	    ActionElement ptr = _actionList;
	    ActionElement trail = null;
	    boolean found = false;

	    while ((ptr != null) && (!found))
	    {
		if (ptr.equals(u))
		    found = true;
		else
		{
		    trail = ptr;
		    ptr = ptr.next();
		}
	    }

	    if (found)
	    {
		if (trail == null)
		{
		    /*
		     * Head of list.
		     */

		    _actionList = _actionList.next();
		}
		else
		    trail.next(ptr.next());
		
		_number--;

		return true;
	    }
	}

	return false;	
    }

public final synchronized BasicAction orderedPop ()
    {
	if (_actionList == null)
	    return null;

	ActionElement ptr = _actionList;
	BasicAction a = ptr.action();
	_actionList = _actionList.next();

	_number--;

	ptr.next(null);
	ptr = null;

	return a;
    }
    
public final synchronized int size ()
    {
	return _number;
    }

private ActionElement _actionList;
private int           _number;
    
}

class ActionElement
{

public ActionElement (BasicAction u)
    {
	_action = u;
	_next = null;
    }

public ActionElement (BasicAction u, ActionElement n)
    {
	_action = u;
	_next = n;
    }    

public final boolean equals (BasicAction u)
    {
	return (boolean) (_action == u);
    }
    
public final ActionElement next ()
    {
	return _next;
    }

public final void next (ActionElement n)
    {
	_next = n;
    }

public final BasicAction action ()
    {
	return _action;
    }
    
private BasicAction  _action;
private ActionElement _next;
    
}
