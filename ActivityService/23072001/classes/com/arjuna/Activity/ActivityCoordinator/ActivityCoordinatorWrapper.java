/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ActivityCoordinatorWrapper.java,v 1.1 2001/07/23 13:44:53 nmcl Exp $
 */

package com.arjuna.Activity.ActivityCoordinator;

import com.arjuna.Activity.ActivityReaper;
import com.arjuna.Activity.SignalSets.*;
import com.arjuna.ArjunaCommon.Common.Uid;
import com.arjuna.CosTransactions.OTS;
import com.arjuna.CosTransactions.OTS_Current;
import com.arjuna.CosTransactions.OTS_ControlWrapper;
import org.omg.CosTransactions.*;
import org.omg.CosActivity.*;
import java.util.*;

import org.omg.CORBA.BAD_OPERATION;
import org.omg.CORBA.BAD_PARAM;
import org.omg.CORBA.SystemException;

/*
 * We only actually create an ActivityCoordinator when the first
 * action is registered. This class acts as a place-holder until
 * then.
 *
 * The only potential problem with this is that the enclosing
 * transaction hierarchy may change between the activity being
 * "created" and us actually creating the activity!
 */

public class ActivityCoordinatorWrapper
{

public ActivityCoordinatorWrapper (ActivityCoordinatorWrapper parent,
				   int timeout)
    {
	_parentWrapper = parent;
	_coordImple = null;
	_coordRef = null;
	_timeout = timeout;
	_failOnly = false;
	_enclosingTransaction = getEnclosingTx();
	_valid = true;
	_completed = false;
	_theUid = new Uid();
	
	/*
	 * If there is a parent then broadcast the ChildLifetime SignalSet.
	 */

	if (_parentWrapper != null)
	{
	    /*
	     * This is different to the addChildActivity call later, because
	     * the ActivityCoordinator hasn't actually been created at this
	     * point, but we still need to tell the parent it has!
	     */

	    try
	    {
		_parentWrapper.doChildBegin(get_global_id());
	    }
	    catch (Exception e)
	    {
		_failOnly = true;
		_valid = false;
	    }
	}

	/*
	 * Now add to the reaper.
	 */

	if (_timeout >= 0)
	{
	    ActivityReaper reaper = ActivityReaper.create();
	    
	    if (reaper != null)
	    {
		if (!reaper.insert(this))
		    System.err.println("WARNING - could not add activity "+get_uid()+" to reaper");
	    }
	    else
		System.err.println("WARNING - no activity reaper present!");
	}
    }

public final boolean valid ()
    {
	return _valid;
    }

public final Uid get_uid ()
    {
	return _theUid;
    }
    
public final boolean isLocal ()
    {
	if (_coordImple != null)
	    return true;
	else
	{
	    /*
	     * If lazily creating the coordinator then we are
	     * local!
	     */

	    if ((_coordImple == null) && (_coordRef == null))
		return true;
	    else
		return false;
	}
    }

public void destroy () throws AlreadyDestroyed
    {
	if (_coordImple != null)
        {
	    _coordImple.destroy();
	    _coordImple = null;
	}
    }

public final org.omg.CosActivity.Status get_status () throws SystemException
    {
	if (_completed)
	    return org.omg.CosActivity.Status.StatusCompleted;
	
	if ((_coordImple == null) && (_coordRef == null)) // no coordinator created yet
	{
	    return org.omg.CosActivity.Status.StatusActive;
	}
	else
	{
	    if (_coordImple != null)
		return _coordImple.get_status();
	    else
		return _coordRef.get_status();
	}
    }

    /*
     * If the activity has not been created yet then the default
     * completion status is CompletionStatusFail (see specification).
     */

public final org.omg.CosActivity.CompletionStatus get_completion_status ()
    {
	if (_valid && (_coordImple != null))
	    return _coordImple.getCompletionStatus();
	else
	{
	    if (_valid)
		return CompletionStatus.CompletionStatusFail;
	    else
		return CompletionStatus.CompletionStatusFailOnly;
	}
    }

public final void set_completion_status (CompletionStatus cs) throws SystemException
    {
	createActivityCoordinator();  // null op if already done

	if (_valid && (_coordImple != null))
	    _coordImple.setCompletionStatus(cs);
	else
	    throw new BAD_OPERATION();  // should be INVALID_ACTIVITY
    }

public final String get_completion_signal_set ()
    {
	if (_coordImple != null)
	    return _coordImple.getCompletionSignalSet();
	else
	    return null; // shouldn't get here anyway!
    }

public final void set_completion_signal_set (String signalSetName) throws SystemException
    {
	createActivityCoordinator();  // null op if already done

	if (_valid && (_coordImple != null))
	    _coordImple.setCompletionSignalSet(signalSetName);
	else
	    throw new BAD_OPERATION();  // should be INVALID_ACTIVITY
    }
    
public final Outcome complete () throws SignalSetUnknown, ActivityPending, ChildContextPending, SystemException
    {
	if (!_valid || _completed)
	    throw new BAD_OPERATION();  // should be INVALID_ACTIVITY
	else
	{
	    Outcome o = null;

	    try
	    {
		if (_coordImple != null)
		    o = _coordImple.complete();
	    }
	    finally
	    {
		_completed = true;

		if (_timeout >= 0)
		{
		    ActivityReaper reaper = ActivityReaper.create();
	    
		    if (reaper != null)
			reaper.remove(this);
		}
	    }

	    return o;
	}
   }

public final String get_activity_name () throws SystemException
    {
	createActivityCoordinator();  // null op if already done
	
	if (_coordImple != null)
	    return _coordImple.get_activity_name();
	else
	    return _coordRef.get_activity_name();
    }

public final ActivityCoordinatorImple getCoordinatorImple ()
    {
	createActivityCoordinator();  // null op if already done

	return _coordImple;
    }

public final ActivityCoordinator get_coordinator () throws SystemException
    {
	createActivityCoordinator();  // null op if already done

	if ((_coordImple != null) && (_coordRef == null))
	    _coordRef = _coordImple.getReference();
	
	return _coordRef;
    }

public final ActivityCoordinator get_parent_coordinator () throws SystemException
    {
	if (_parentWrapper != null)
	    return _parentWrapper.get_coordinator();
	else
	    return null;
    }

public final int get_timeout ()
    {
	return _timeout;
    }

public final byte[] get_global_id () throws SystemException
    {
	return _theUid.stringForm().getBytes();
    }

public final Outcome complete_activity (String signal_set_name, CompletionStatus cs) throws ActivityPending, ChildContextPending, SignalSetUnknown, SystemException
    {
	createActivityCoordinator();  // null op if already done

	if (_valid && (_coordImple != null))
	{
	    try
	    {
		return _coordImple.complete_activity(signal_set_name, cs);
	    }
	    finally
	    {
		if (_timeout >= 0)
		{
		    ActivityReaper reaper = ActivityReaper.create();
	    
		    if (reaper != null)
			reaper.remove(this);
		}
	    }
	}
	else
	    throw new BAD_OPERATION();
    }

public final Outcome process_signal_set (String signal_set_name, CompletionStatus cs) throws SignalSetUnknown, SystemException
    {
	createActivityCoordinator();  // null op if already done

	if (_valid && (_coordImple != null))
	    return _coordImple.process_signal_set(signal_set_name, cs);
	else
	    throw new BAD_OPERATION();
    }
	
public final void add_signal_set (SignalSet signal_set) throws SignalSetAlreadyRegistered, SystemException
    {
	createActivityCoordinator();  // null op if already done

	if (_valid && (_coordImple != null))
	    _coordImple.add_signal_set(signal_set);
	else
	    throw new BAD_OPERATION();
    }
    
public final void remove_signal_set (String signal_set_name) throws SignalSetUnknown, SystemException
    {
	if (!_valid)
	    throw new BAD_OPERATION();

	if (_coordImple != null)
	    _coordImple.remove_signal_set(signal_set_name);
	else
	    throw new SignalSetUnknown();
    }
	
public final void add_action (Action act, String signal_set_name, int priority) throws SignalSetUnknown, SystemException
    {
	createActivityCoordinator();  // null op if already done

	if (_valid && (_coordImple != null))
	    _coordImple.add_action(act, signal_set_name, priority);
	else
	    throw new BAD_OPERATION();
    }
    
public final void remove_action (Action act) throws ActionNotFound, SystemException
    {
	if (!_valid)
	    throw new BAD_OPERATION();

	if (_coordImple != null)
	    _coordImple.remove_action(act);
	else
	    throw new ActionNotFound();
    }
    
public final void add_actions (Action[] acts, String signal_set_name, int priority) throws SignalSetUnknown, SystemException
    {
	createActivityCoordinator();  // null op if already done

	if (_valid && (_coordImple != null))
	    _coordImple.add_actions(acts, signal_set_name, priority);
	else
	    throw new BAD_OPERATION();
    }
    
public final Action[] remove_actions (Action[] acts) throws SystemException
    {
	if (!_valid)
	    throw new BAD_OPERATION();

	if (_coordImple != null)
	    return _coordImple.remove_actions(acts);
	else
	    return acts;  // all actions not found!!
    }
    
public final void add_global_action (Action act, int priority) throws SystemException
    {
	createActivityCoordinator();  // null op if already done

	if (_valid && (_coordImple != null))
	    _coordImple.add_global_action(act, priority);
	else
	    throw new BAD_OPERATION();
    }
    
public final void remove_global_action (Action act) throws ActionNotFound, SystemException
    {
	if (!_valid)
	    throw new BAD_OPERATION();

	if (_coordImple != null)
	    _coordImple.remove_global_action(act);
	else
	    throw new ActionNotFound();
    }
    
public final int get_number_registered_actions (String signal_set_name) throws SignalSetUnknown, SystemException
    {
	if (!_valid)
	    throw new BAD_OPERATION();

	if (_coordImple != null)
	    return _coordImple.get_number_registered_actions(signal_set_name);
	else
	    return 0;
    }
	
public final Action[] get_actions (String signal_set_name) throws SignalSetUnknown, SystemException
    {
	if (!_valid)
	    throw new BAD_OPERATION();

	if (_coordImple != null)
	    return _coordImple.get_actions(signal_set_name);
	else
	    return null;
    }

public boolean equals (java.lang.Object o)
    {
	if (o == null)
	    return false;
	
	if (o instanceof ActivityCoordinatorWrapper)
	{
	    if (o == this)
		return true;
	    else
	    {
		ActivityCoordinatorWrapper act = (ActivityCoordinatorWrapper) o;
		
		if (act.getCoordinatorImple() == getCoordinatorImple())
		    return true;
	    }
	}
	
	return false;
    }

public final void addTransaction (OTS_ControlWrapper tx) throws SystemException
    {
	 createActivityCoordinator();  // null op if already done
	 
	 if (_coordImple != null)
	     _coordImple.addTransaction(tx);
	 else
	     throw new BAD_OPERATION();
    }
	
public final void removeTransaction (OTS_ControlWrapper tx) throws SystemException
    {
	 if (_coordImple != null)
	     _coordImple.removeTransaction(tx);
	 else
	     throw new BAD_OPERATION();	
    }
    
public final ActivityCoordinatorWrapper getParentCoordinator ()
    {
	return _parentWrapper;
    }

public final Outcome getOutcome ()
    {
	return ((_coordImple != null) ? _coordImple.get_outcome() : null);
    }
    
public final boolean isTransactional ()
    {
	Vector v = getEnclosedTransactions();
	
	if ((v != null) && (v.size() > 0))
	    return true;
	else
	    return false;
    }
    
public final Control getEnclosingTransaction ()
    {
	return _enclosingTransaction;
    }
    
public final Vector getEnclosedTransactions ()
    {
	if (_coordImple != null)
	    return _coordImple.getEnclosedTransactions();
	else
	    return null;
    }

    /*
     * Could cache these ids and only pass them to the actual coordinator
     * when it is created.
     */

public final synchronized void addThread (Thread ct)
     {
	 createActivityCoordinator();  // null op if already done

	 if (_coordImple != null)
	     _coordImple.addThread(ct);
	 else
	     throw new BAD_OPERATION();
     }

public final synchronized void removeThread (Thread ct)
     {
	 if (_coordImple != null)
	     _coordImple.removeThread(ct);
	 else
	     throw new BAD_OPERATION();
     }

final void addChildActivity (ActivityCoordinatorImple coord) throws SystemException
     {
	 createActivityCoordinator();  // null op if already done
	 
	 if (_coordImple != null)
	     _coordImple.addChildActivity(coord);
	 else
	     throw new BAD_OPERATION();
     }

final void removeChildActivity (ActivityCoordinatorImple coord) throws SystemException
     {
	 if (_coordImple != null)
	     _coordImple.removeChildActivity(coord);
	 else
	     throw new BAD_OPERATION();
     }

private final void createActivityCoordinator ()
    {
	if ((_coordImple == null) && (_coordRef == null))
	{
	    if (_parentWrapper == null)
	    {
		if (_enclosingTransaction != null)
		    _coordImple = ActivityFactory.factory().create(_theUid, _timeout, _enclosingTransaction);
		else
		    _coordImple = ActivityFactory.factory().create(_theUid, _timeout);
	    }
	    else
	    {
		_coordImple = ActivityFactory.factory().createSubactivity(_theUid, _parentWrapper.getCoordinatorImple(), _timeout);
	    }

	    /*
	     * Did an error happened before we created the activity?
	     * Typically we could not get the enclosing transaction
	     * context for this activity when it was "created".
	     */

	    if (_failOnly)
	    {
		try
		{
		    _coordImple.setCompletionStatus(CompletionStatus.CompletionStatusFailOnly);
		}
		catch (Exception e)
		{
		    /*
		     * Could not set the completion status!
		     */

		    System.err.println(e);

		    _valid = false;
		}
	    }
	}
    }

private final Control getEnclosingTx ()
    {
	OTS_Current current = OTS.current();
	
	if (current != null)
	{
	    try
	    {
		if (current.get_status() == org.omg.CosTransactions.Status.StatusActive)
		{
		    return current.get_control();
		}
	    }
	    catch (SystemException e)
	    {
		_failOnly = true;
	    }
	}
	
	return null;
    }

/*
 * Since the wrapper is intended to allow the delay of the creation
 * of the actual coordinator, we do child begin and child complete
 * signalling within the wrapper. If we left it until the coordinator
 * was created, it may be too late.
 *
 * ChildLifetime signal processing is slightly different since each
 * SignalSet is specific to a given child activity. Rather than store
 * each created SignalSet for the duration of the activity (so we can
 * return to it for the childComplete signal) we simply create a new
 * "one-shot" SignalSet each time.
 */
 
private final void doChildBegin (byte[] child) throws SignalSetUnknown, SystemException
    {
	if ((_coordImple == null) || (_coordImple._actionList == null))
	    return;
	
	if (child == null)
	    throw new BAD_PARAM();

	Vector actions = _coordImple._actionList.getActions(com.arjuna.Activity.SignalSets.SystemDefined.ChildLifetime);
	Vector globalActions = _coordImple._actionList.getGlobalActions();

	/*
	 * Don't do anything if no Actions are registered. Means we can put
	 * off creation of coordinator, since get_global_id causes it to
	 * be created. (Currently!)
	 */

	if (((actions != null) && (actions.size() > 0)) ||
	    ((globalActions != null) && (globalActions.size() > 0)))
	{
	    java.lang.Object[] params = new java.lang.Object[1];
	    
	    params[0] = new String(child);
	    
	    SignalSet ss = _coordImple._signalSetList.getSignalSet(SystemDefined.ChildLifetime, params);
	    
	    if (!_coordImple.driveSignalSet(ss, actions, globalActions, false))
	    {
		// could not signal parent, so fail!
		
		_coordImple.setCompletionStatus(CompletionStatus.CompletionStatusFailOnly);
	    }
	}
    }

private ActivityCoordinatorWrapper _parentWrapper;
private ActivityCoordinatorImple   _coordImple;
private ActivityCoordinator        _coordRef;
private int                        _timeout;
private boolean                    _failOnly;
private Control                    _enclosingTransaction;
private boolean                    _valid;
private boolean                    _completed;
private Uid                        _theUid;
    
};
