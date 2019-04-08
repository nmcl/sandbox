/*
 * Copyright (C) 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ChildLifetime.javatmpl,v 1.1 2001/07/23 13:44:56 nmcl Exp $
 */

/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OrbPortability.h,v 1.1 2000/02/25 14:09:59 nmcl Exp $
 */




/*
 * Try to get around the differences between Ansi CPP and
 * K&R cpp with concatenation.
 */


/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: OrbPortability_stdc.h,v 1.2.4.1.2.3.26.1.2.2.4.2 2001/02/02 11:51:52 nmcl Exp $
 */
































































package com.arjuna.Activity.SignalSets;

import com.arjuna.OrbCommon.*;
import com.arjuna.Activity.ActivityCoordinator.ActivityCoordinatorWrapper;
import com.arjuna.Activity.Signals.*;
import org.omg.CosActivity.CompletionStatus;
import org.omg.CosActivity.*;
import org.omg.CORBA.*;

import org.omg.CosActivity.SignalSetInactive;
import org.omg.CosActivity.SignalSetActive;
import org.omg.CosActivity.AlreadyDestroyed;
import org.omg.CORBA.BAD_PARAM;

// LocalObject and do not activate

public class ChildLifetime extends  org.omg.CosActivity . SignalSetPOA 
{

public ChildLifetime (java.lang.Object[] params)
    {
	_reference = null;
	_active = false;
	_completed = false;
	_completionStatus = CompletionStatus.CompletionStatusFail;
	_destroyed = false;
	_childId = null;

	if ((params == null) || (params.length != 1))
	{
	    System.err.println("ChildLifetime error - insufficient parameters!");

	    _completionStatus = CompletionStatus.CompletionStatusFailOnly;
	    _completed = true;  // better not send anything!
	}
	else
	{
	    try
	    {
		String act = (String) params[0];

		_childId = act.getBytes();
	    }
	    catch (Exception e)
	    {
		e.printStackTrace();

		_completionStatus = CompletionStatus.CompletionStatusFailOnly;
		_completed = true;
	    }
	}
    }
 
public final SignalSet getReference ()
    {
	if (_reference == null)
	{
	    OAInterface.objectIsReady( this ) ;
	    
	    _reference =  org.omg.CosActivity.SignalSetHelper.narrow(OAInterface.corbaReference( this )) ;
	}
	
	return _reference;
    }

public synchronized Signal get_signal (BooleanHolder lastSignal) throws SystemException
    {
	if (!_active && !_completed)
	{
	    _active = true;
	    _completed = true;
	    lastSignal.value = true;

	    return com.arjuna.Activity.Signals.ChildLifetimeBegin.getSignal(_childId, _completionStatus);
	}
	else
	    return null;
    }
 
public synchronized Outcome get_outcome () throws SignalSetActive, SystemException
    {
	if (!_completed)
	    throw new SignalSetActive();
	else
	    return null;
    }

public boolean set_response (Outcome response,
			     BooleanHolder nextSignal) throws SignalSetInactive, SystemException
    {
	if (!_active)
	    throw new SignalSetInactive();
	
	if (response == null)
	    throw new BAD_PARAM();
	else
	{
	    if (response.outcome_name.equals("ActionError") ||
		response.outcome_name.equals("ActionSystemException"))
	    {
		if (!_completed)
		{
		    /*
		     * An error has occurred, so tell ActivityCoordinator
		     * to send the next Signal.
		     */

		    _completionStatus = CompletionStatus.CompletionStatusFailOnly;
		    nextSignal.value = true;

		    if (response.outcome_name.equals("ActionSystemException"))
			return false;
		    else
			return true;
		}
		else
		{
		    /*
		     * Otherwise, ignore as we have no other Signal to
		     * send, and postCompletion is the last Signal ever
		     * in an Activity's lifetime.
		     */

		    return true;
		}
	    }
	    else
	    {
		/*
		 * No error has occurred, so don't change the completion
		 * status, and allow the ActivityCoordinator to keep sending
		 * the same signal to the other Actions.
		 */

		nextSignal.value = false;
	    }
	}

	return false;
    }
 
public void set_completion_status (CompletionStatus cs) throws SystemException
    {
	_completionStatus = cs;
    }
 
public CompletionStatus get_completion_status () throws SystemException
    {
	return _completionStatus;
    }

/*
 * In case we need the reference. We don't, so ignore. But throw an
 * exception if we are used invalidly.
 */
 
public void set_activity_coordinator (ActivityCoordinator coord) throws SignalSetActive, SystemException
    {
	if (_active)
	    throw new SignalSetActive();
    }
 
public synchronized void destroy () throws AlreadyDestroyed, SystemException
    {
	if (!_destroyed)
	{
	    _destroyed = true;
	    _reference = null;
	    OAInterface.shutdownObject( this ) ;
	}
	else
	    throw new AlreadyDestroyed();
    }

public String signal_set_name () throws SystemException
    {
	return SystemDefined.ChildLifetime;
    }

private SignalSet        _reference;
private boolean          _active;
private boolean          _completed;
private CompletionStatus _completionStatus;
private boolean          _destroyed;
private byte[]           _childId;
 
};
