/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ActionList.java,v 1.1 2001/07/23 13:44:52 nmcl Exp $
 */

package com.arjuna.Activity.ActivityCoordinator;

import com.arjuna.Activity.Signals.*;
import com.arjuna.Activity.SignalSets.*;
import org.omg.CosTransactions.*;
import org.omg.CosActivity.CompletionStatus;
import org.omg.CosActivity.*;
import org.omg.CORBA.*;
import java.util.*;

import org.omg.CosActivity.SignalSetAlreadyRegistered;
import org.omg.CosActivity.ActionNotFound;
import org.omg.CORBA.BAD_PARAM;
import org.omg.CORBA.SystemException;
import java.lang.ArrayIndexOutOfBoundsException;

/*
 * This object maintains the lists of Actions and the names of the SignalSets
 * they have been registered with, and the global Actions (these are Actions
 * that receive every Signal sent).
 */

class ActionList
{

public ActionList ()
    {
	_actions = null;
	_globalActions = null;
    }

public synchronized void addAction (Action act, String signal_set_name,
				    int priority) throws SystemException
    {
	if ((act == null) || (signal_set_name == null) || (priority < 0))
	    throw new BAD_PARAM();

	Vector v = null;
	
	if (_actions != null)
	    v = (Vector) _actions.get(signal_set_name);
		
	if (v == null)
	{
	    v = new Vector();
	    
	    if (_actions == null)
		_actions = new Hashtable();
	    
	    _actions.put(signal_set_name, v);
	}
	
	/*
	 * Shuffles elements up to make room?
	 */

	if (v.size() < priority)
	    v.setSize(priority+1);

	try
	{
	    if (v.elementAt(priority) != null)
	    {
		v.setSize(v.size()+1);
		
		for (int i = priority; i < v.size()-1; i++)
		    v.insertElementAt(v.elementAt(i), i+1);
	    }
	}
	catch (ArrayIndexOutOfBoundsException e)
	{
	    // nothing there!
	}
	
	v.insertElementAt(act, priority);
    }
 
public synchronized void removeAction (Action act) throws ActionNotFound, SystemException
    {
	if (act == null)
	    throw new BAD_PARAM();
	
	if (_actions == null)
	    throw new ActionNotFound();
	
	Enumeration e = _actions.elements();
		
	while (e.hasMoreElements())
	{
	    Vector v = (Vector) e.nextElement();
		    
	    if (v != null)
	    {
		if (v.removeElement(act))
		    return;
	    }
	}

	/*
	 * If we get here then the action was not present in any
	 * of the signal set lists!
	 */

	throw new ActionNotFound();
    }
 
public synchronized void addActions (Action[] acts, String signal_set_name,
				     int priority) throws SystemException
    {
	if ((acts == null) || (signal_set_name == null) || (priority < 0))
	    throw new BAD_PARAM();

	Vector v = null;
	
	if (_actions != null)
	    v = (Vector) _actions.get(signal_set_name);
		
	if (v == null)
	{
	    v = new Vector();

	    if (_actions == null)
		_actions = new Hashtable();

	    _actions.put(signal_set_name, v);
	}

	if (v.size() < priority)
	    v.setSize(priority);

	try
	{
	    if (v.elementAt(priority) != null)
	    {
		v.setSize(v.size()+acts.length);
		
		for (int i = priority; i < v.size()-acts.length; i++)
		    v.insertElementAt(v.elementAt(i), i+acts.length);
	    }
	}
	catch (ArrayIndexOutOfBoundsException e)
	{
	    // nothing there!
	}

	int index = 0;
	    
	for (int i = 0; i < acts.length; i++)
	{
	    /*
	     * Shuffles elements up to make room?
	     */
	    
	    v.insertElementAt(acts[i], priority+index);
	    index++;
	}
    }
 
public Action[] removeActions (Action[] acts) throws SystemException
    {
	if (acts == null)
	    throw new BAD_PARAM();
	
	Vector failed = null;

	for (int i = 0; i < acts.length; i++)
	{
	    try
	    {
		removeAction(acts[i]);
	    }
	    catch (ActionNotFound e)
	    {
		if (failed == null)
		    failed = new Vector();
		
		failed.addElement(acts);
	    }
	}

	if (failed != null)
	{
	    Action[] failures = new Action[failed.size()];
	    
	    for (int j = 0; j < failed.size(); j++)
		failures[j] = (Action) failed.elementAt(j);

	    return failures;
	}
	else
	    return null;
    }

public synchronized void addGlobalAction (Action act,
					  int priority) throws SystemException
    {
	if ((act == null) || (priority < 0))
	    throw new BAD_PARAM();
	
	if (_globalActions == null)
	    _globalActions = new Vector();

	/*
	 * Shuffles elements up to make room?
	 */

	if (_globalActions.size() < priority)
	    _globalActions.setSize(priority);

	try
	{
	    if (_globalActions.elementAt(priority) != null)
	    {
		_globalActions.setSize(_globalActions.size()+1);
		
		for (int i = priority; i < _globalActions.size()-1; i++)
		    _globalActions.insertElementAt(_globalActions.elementAt(i), i+1);
	    }
	}
	catch (ArrayIndexOutOfBoundsException e)
	{
	    // nothing there!
	}

	_globalActions.insertElementAt(act, priority);
    }
 
public synchronized void removeGlobalAction (Action act) throws ActionNotFound, SystemException
    {
	if (act == null)
	    throw new BAD_PARAM();
	
	if ((_globalActions == null) || (!_globalActions.removeElement(act)))
	    throw new ActionNotFound();
    }
 
public synchronized int getNumberRegisteredActions (String signal_set_name) throws SystemException
    {
	if (signal_set_name == null)
	    throw new BAD_PARAM();
	
	Vector v = null;
	
	if (_actions != null)
	    v = (Vector) _actions.get(signal_set_name);

	return ((v == null) ? 0 : v.size());
    }
 
public synchronized Vector getActions (String signalSetName)
    {
	if (signalSetName == null)
	    throw new BAD_PARAM();
	
	return ((_actions != null) ? (Vector) _actions.get(signalSetName) : null);
    }

public synchronized Action[] get_actions (String signalSetName)
    {
	Vector v = getActions(signalSetName);
	
	if (v != null)
	{
	    Action[] act = new Action[v.size()];
	    
	    for (int i = 0; i < act.length; i++)
		act[i] = (Action) v.elementAt(i);

	    return act;
	}
	else
	    return null;
    }
    
public synchronized Vector getGlobalActions ()
    {
	return _globalActions;
    }

private Hashtable _actions;
private Vector    _globalActions;

};
