/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ActivityReaper.java,v 1.1 2001/07/23 13:44:49 nmcl Exp $
 */

package com.arjuna.Activity;

import com.arjuna.Activity.ActivityCoordinator.ActivityCoordinatorWrapper;
import com.arjuna.ArjunaCommon.Common.PropertyManager;
import com.arjuna.OrbCommon.*;
import com.arjuna.ArjunaCommon.Template.*;
import org.omg.CosActivity.*;

import org.omg.CosActivity.ActivityPending;
import org.omg.CosActivity.ChildContextPending;
import org.omg.CosActivity.SignalSetUnknown;
import org.omg.CORBA.SystemException;

// take TeT reaper code.

/**
 * Class to record activities with non-zero timeout values, and
 * class to implement a reaper thread which terminates
 * these activities once their timeout elapses.
 */

class ReaperElement implements OrderedListElement
{

    /*
     * Currently, once created the reaper object and thread stay around
     * forever. We could destroy both once the list of transactions is null.
     * Depends upon the relative cost of recreating them over keeping them
     * around.
     */

public ReaperElement (ActivityCoordinatorWrapper activity)
    {
	_activity = activity;

	/*
	 * Given a timeout period in seconds, calculate its absolute value
	 * from the current time of day in milliseconds.
	 */
	
	_absoluteTimeout = _activity.get_timeout()*1000 + System.currentTimeMillis();
    }

public void finalize ()
    {
	_activity = null;
    }
    
public final boolean equals (OrderedListElement e)
    {
	if (e instanceof ReaperElement)
	    return (_absoluteTimeout == ((ReaperElement) e)._absoluteTimeout);
	else
	    return false;
    }

public final boolean lessThan (OrderedListElement e)
    {
	if (e instanceof ReaperElement)
	    return (_absoluteTimeout < ((ReaperElement)e)._absoluteTimeout);
	else
	    return false;
    }

public final boolean greaterThan (OrderedListElement e)
    {
	if (e instanceof ReaperElement)
	    return (_absoluteTimeout > ((ReaperElement)e)._absoluteTimeout);
	else
	    return false;
    }

protected ActivityCoordinatorWrapper _activity;
protected long                     _absoluteTimeout;
    
};


public class ActivityReaper
{

public static final String NORMAL = "NORMAL";
public static final String DYNAMIC = "DYNAMIC";

public ActivityReaper (long checkPeriod)
    {
	_list = new OrderedList();
	_checkPeriod = checkPeriod;
    }

public void finalize ()
    {
	_list = null;
    }

public final long checkingPeriod ()
    {
	return _checkPeriod;
    }

    /*
     * Should be no need to protect with a mutex since only one thread
     * is ever doing the work.
     */

    /**
     * Only check for one at a time to prevent starvation.
     *
     * Timeout is given in milliseconds.
     */
    
public final boolean check (long timeout)
    {
	if (_list == null)
	    return true;
	
	OrderedListIterator iter = new OrderedListIterator(_list);
	ReaperElement e = null;

	while ((e = (ReaperElement) iter.iterate()) != null)
	{
	    if (timeout >= e._absoluteTimeout)
		break;
	    else
	    {
		iter = null;
		return true;
	    }
	}

	iter = null;
	
	if (e != null)
	{
	    /*
	     * Currently we lazily determine that activities may have
	     * previously terminated but may still be on our list.
	     */

	    org.omg.CosActivity.Status stat = org.omg.CosActivity.Status.StatusActive;

	    try
	    {
		stat = e._activity.get_status();
	    }
	    catch (Exception ex1)
	    {
		System.err.println(ex1);
		
		return false;
	    }

	    /*
	     * Only force rollback if the transaction is still running.
	     */
	    
	    if (stat == org.omg.CosActivity.Status.StatusActive)
	    {
		boolean problem = true;
		
		try
		{
		    e._activity.set_completion_status(org.omg.CosActivity.CompletionStatus.CompletionStatusFailOnly);
		    e._activity.complete();

		    /*
		     * Could also call destroy, but currently we leave
		     * that up to Current, although there is no real
		     * reason why we shouldn't do it here!
		     */

		    problem = false;
		}
		catch (ActivityPending e1)
		{
		}
		catch (ChildContextPending e2)
		{
		}
		catch (SignalSetUnknown e3)
		{
		}
		catch (SystemException e4)
		{
		    /*
		     * Unknown, so leave on list.
		     */
		}

		/*
		 * Set completion status.
		 */

		if (problem)
		{
		    try
		    {
			e._activity.set_completion_status(org.omg.CosActivity.CompletionStatus.CompletionStatusFailOnly);
		    }
		    catch (Exception ex)
		    {
		    }
		}
		else
		{
		    /*
		     * Can remove from list.
		     */
		
		    _list.remove(e);
		}
	    }
	    else
		_list.remove(e);  // not active, so remove anyway.
	}

	System.gc();  // do some garbage collection while we're at it!
	
	return true;
    }
    
    /*
     * timeout is given in seconds, but we work in milliseconds.
     */
 
public final synchronized boolean insert (ActivityCoordinatorWrapper activity)
    {
	if (activity == null)
	    return false;
	
	/*
	 * Ignore if the timeout is -1, since this means the activity
	 * should never timeout.
	 */

	int timeout = activity.get_timeout();
	
	if (timeout == -1)
	    return true;
    
	if ((timeout < _checkPeriod) || (_checkPeriod == -1))
	{
	    _checkPeriod = timeout;
	    ActivityReaper._reaperThread.interrupt();
	}
	
	ReaperElement e = new ReaperElement(activity);

	if ((_list != null) && _list.insert(e))
	    return true;
	else
	{
	    e = null;
	    return false;
	}
    }

public final synchronized boolean remove (ActivityCoordinatorWrapper activity)
    {
	if ((_list == null) || (activity == null))
	    return false;
    
	ReaperElement e = null;
	OrderedListIterator iter = new OrderedListIterator(_list);

	while ((e = (ReaperElement) iter.iterate()) != null)
	{
	    if (e._activity == activity)
	    {
		_list.remove(e);
		
		return true;
	    }
	}

	return false;
    }

    /*
     * Currently we let the reaper thread run at same priority as other
     * threads. Could get priority from environment.
     */

public static synchronized ActivityReaper create (long checkPeriod)
    {
	if (ActivityReaper._theReaper == null)
	{
	    String mode = PropertyManager.getProperty(Environment.ACTIVITY_REAPER_MODE);
	    
	    if (mode != null)
	    {
		if (mode.compareTo(ActivityReaper.DYNAMIC) == 0)
		    ActivityReaper._dynamic = true;
	    }
	    
	    if (!ActivityReaper._dynamic)
	    {
		String timeoutEnv = PropertyManager.getProperty(Environment.ACTIVITY_REAPER_TIMEOUT);

		if (timeoutEnv != null)
		{
		    Long l = null;
		
		    try
		    {
			l = new Long(timeoutEnv);
			checkPeriod = l.longValue()/1000;  // convert microseconds to milliseconds
			
			l = null;
		    }
		    catch (NumberFormatException e)
		    {
			System.err.println("ActivityReaper::create - "+e);
		    }
		}
	    }
	    else
		checkPeriod = -1;
		
	    ActivityReaper._theReaper = new ActivityReaper(checkPeriod);
	    
	    _reaperThread = new ReaperThread(ActivityReaper._theReaper);
	    _reaperThread.setPriority(Thread.MIN_PRIORITY);

	    ORBInterface.addPreORBShutdown(new ReaperCleanup(_reaperThread));
	    
	    _reaperThread.start();
	}

	return ActivityReaper._theReaper;
    }

public static synchronized ActivityReaper create ()
    {
	return create(ActivityReaper.defaultCheckPeriod);
    }
    
public static synchronized ActivityReaper activityReaper ()
    {
	return _theReaper;
    }
    
public static final long defaultCheckPeriod = 120000;  // in milliseconds
    
private OrderedList _list;
private long        _checkPeriod;
    
private static ActivityReaper _theReaper = null;
private static ReaperThread   _reaperThread = null;
private static boolean        _dynamic = false;
    
};

class ReaperThread extends Thread
{

public ReaperThread (ActivityReaper arg)
    {
	reaperObject = arg;
	sleepPeriod = reaperObject.checkingPeriod();
	_shutdown = false;
    }
    
public void run ()
    {
	for (;;)
	{
	    /*
	     * Cannot assume we sleep for the entire period. We may
	     * be interrupted. If we are, just run a check anyway and
	     * ignore.
	     */

	    boolean done = false;
	    
	    while (!done)
	    {
		long oldPeriod = sleepPeriod;

		try
		{
		    Thread.sleep(sleepPeriod);

		    done = true;
		}
		catch (InterruptedException e1)
		{
		    /*
		     * Has timeout been changed?
		     */

		    if (reaperObject.checkingPeriod() == oldPeriod)
		    {
			done = true;
		    }
		}
		catch (Exception e2)
		{
		    done = true;
		}
	    }

	    if (_shutdown)
		return;

	    reaperObject.check(System.currentTimeMillis());
	}
    }

public void shutdown ()
    {
	_shutdown = true;
    }

private ActivityReaper reaperObject;
private long sleepPeriod;
private boolean _shutdown;

};


/*
 * A Java application may not cleanly exit if a thread
 * is still running, so we must explicitly kill the terminating
 * thread. We could get it to terminate any outstanding transactions
 * as well. (Only those with non-zero timeouts!) We don't at the
 * moment, since some of these could be remote.
 */

class ReaperCleanup extends ORBPreShutdown
{

public ReaperCleanup (ReaperThread thread)
    {
	super("ReaperCleanup");
	
	_reaperThread = thread;
    }

public void finalize ()
    {
	if (_reaperThread != null)
	    work();
    }

public void work ()
    {
	if (_reaperThread != null)
	{
	    _reaperThread.shutdown();
	    _reaperThread.interrupt();
	    _reaperThread = null;
	}
    }

private ReaperThread _reaperThread;
    
};
