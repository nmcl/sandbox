/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: OTS_TransactionReaper.java,v 1.3.2.1.4.1 2000/05/22 10:23:53 nmcl Exp $
 */

package com.arjuna.CosTransactions;

import com.arjuna.OrbCommon.ORBInterface;

import com.arjuna.OrbCommon.*;
import com.arjuna.ArjunaLite.Atomic.BasicAction;
import com.arjuna.ArjunaLite.Atomic.ActionStatus;
import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaCommon.Template.*;
import org.omg.CosTransactions.*;
import com.arjuna.ArjunaOTS.*;

import com.arjuna.ArjunaOTS.ActiveThreads;
import com.arjuna.ArjunaOTS.ActiveTransaction;
import com.arjuna.ArjunaOTS.BadControl;
import com.arjuna.ArjunaOTS.Destroyed;
import java.lang.NumberFormatException;

/**
 * Class to record transactions with non-zero timeout values, and
 * class to implement a transaction reaper thread which terminates
 * these transactions once their timeout elapses.
 */

/*
 * Rather than associate the timeout with the Control, we keep
 * it separate, and held by the reaper. So, a transaction never
 * knows its timeout.
 */

class ReaperElement implements OrderedListElement
{

    /*
     * Currently, once created the reaper object and thread stay around forever.
     * We could destroy both once the list of transactions is null. Depends upon
     * the relative cost of recreating them over keeping them around.
     */

public ReaperElement (OTS_ControlWrapper control, int timeout)
    {
	_control = control;
	_timeout = timeout;

	/*
	 * Given a timeout period in seconds, calculate its absolute value
	 * from the current time of day in milliseconds.
	 */
	
	_absoluteTimeout = timeout*1000 + System.currentTimeMillis();
    }

public void finalize ()
    {
	_control = null;
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

protected OTS_ControlWrapper _control;
protected long               _absoluteTimeout;
protected int                _timeout;
    
};


public class OTS_TransactionReaper
{

public static final String NORMAL = "NORMAL";
public static final String DYNAMIC = "DYNAMIC";

public OTS_TransactionReaper (long checkPeriod)
    {
	_list = new OrderedList();
	_checkPeriod = checkPeriod;

	if (_list == null)
	{
	    System.err.println("OTS_TransactionReaper - could not create transaction list. Out of memory.");
	    System.exit(0);
	}
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
	     * Only force rollback if the transaction is still running.
	     */
	    
	    if (e._control.get_status() == org.omg.CosTransactions.Status.StatusActive)
	    {
		/*
		 * If this is a local transaction, then we can roll it
		 * back completely. Otherwise, just mark it as rollback only.
		 */

		if (e._control.isLocal())
		{
		    OTS_Transaction transaction = e._control.getImple().getImplHandle();

		    if (transaction != null)
		    {
			try
			{
			    transaction.forceRollback();
			}
			catch (Exception ex2)
			{
			    System.err.println("WARNING: transaction reaper failed to force rollback on "+transaction);
			}
		    }
		    else
			System.err.println("WARNING: transaction reaper with null transaction!");
		}
		else
		{
		    boolean error = false;
		    boolean printDebug = DebugController.controller().enabled();

		    try
		    {
			e._control.rollback_only();
		    }
		    catch (Exception ex3)
		    {
			error = true;
		    }

		    if (error || printDebug)
		    {
			String name = null;

			try
			{
			    name = e._control.get_transaction_name();
			}
			catch (Exception ex4)
			{
			    name = "**unknown**";
			}

			if (error)
			{
			    System.err.println("WARNING OTS_TransactionReaper failed to rollback_only "+name);
			}
			else
			    System.out.println("OTS_TransactionReaper - rolledback "+name);

			name = null;
		    }
		}
	    }
	    
	    _list.remove(e);

	    /*
	     * Although we have terminated the transaction we do not
	     * destroy the control since other threads may still have
	     * references to it and we want them to know what happened.
	     */

	    e = null;
	}

	System.gc();  // do some garbage collection while we're at it!
	
	return true;
    }
    
    /*
     * timeout is given in seconds, but we work in milliseconds.
     */
 
public final synchronized boolean insert (OTS_ControlWrapper control, int timeout)
    {
	/*
	 * Ignore if the timeout is zero, since this means the transaction
	 * should never timeout.
	 */

	if (timeout == 0)
	    return true;
    
	if ((timeout < _checkPeriod) || (_checkPeriod == -1))
	{
	    _checkPeriod = timeout;
	    OTS_TransactionReaper.reaperThread.interrupt();
	}
	
	ReaperElement e = new ReaperElement(control, timeout);

	if ((_list != null) && _list.insert(e))
	    return true;
	else
	{
	    e = null;
	    return false;
	}
    }

public final synchronized boolean remove (OTS_Control control)
    {
	return remove(new OTS_ControlWrapper((OTS_Control) control));
    }

public final synchronized boolean remove (Control control)
    {
	return remove(new OTS_ControlWrapper((Control) control));
    }
    
public final synchronized boolean remove (OTS_ControlWrapper control)
    {
	if ((_list == null) || (control == null))
	    return false;
    
	ReaperElement e = null;
	OrderedListIterator iter = new OrderedListIterator(_list);
	boolean result = false;
	boolean found = false;
	
	while (((e = (ReaperElement) iter.iterate()) != null) && !found)
	{
	    try
	    {
		found = e._control.equals(control);
	    }
	    catch (Exception e2)
	    {
		break;
	    }
	}

	iter = null;

	if (found)
	{
	    result = _list.remove(e);

	    e = null;
	}

	return result;
    }

    /**
     * Given a Control, return the associated timeout, or 0 if
     * we do not know about it.
     *
     * Return in seconds!
     */

public final synchronized int getTimeout (Control control)    
    {
	return getTimeout(new OTS_ControlWrapper((Control) control));
    }
    
public final synchronized int getTimeout (OTS_ControlWrapper control)
    {
	if ((_list == null) || (control == null))
	    return 0;
    
	ReaperElement e = null;
	OrderedListIterator iter = new OrderedListIterator(_list);
	
	while ((e = (ReaperElement) iter.iterate()) != null)
	{
	    try
	    {
		if (e._control.equals(control))
		{
		    iter = null;
		    
		    return e._timeout;
		}
	    }
	    catch (Exception e2)
	    {
		break;
	    }
	}

	return 0;
    }
    
    /*
     * Currently we let the reaper thread run at same priority as other
     * threads. Could get priority from environment.
     */

public static synchronized OTS_TransactionReaper create (long checkPeriod)
    {
	if (OTS_TransactionReaper._theReaper == null)
	{
	    String mode = PropertyManager.getProperty(OTS_Environment.OTS_TX_REAPER_MODE);
	    
	    if (mode != null)
	    {
		if (mode.compareTo(OTS_TransactionReaper.DYNAMIC) == 0)
		    OTS_TransactionReaper._dynamic = true;
	    }
	    
	    if (!OTS_TransactionReaper._dynamic)
	    {
		String timeoutEnv = PropertyManager.getProperty(OTS_Environment.OTS_TX_REAPER_TIMEOUT);

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
			System.err.println("OTS_TransactionReaper::create - "+e);
		    }
		}
	    }
	    else
		checkPeriod = -1;
		
	    OTS_TransactionReaper._theReaper = new OTS_TransactionReaper(checkPeriod);
	    
	    reaperThread = new ReaperThread(OTS_TransactionReaper._theReaper);
	    //	    reaperThread.setPriority(Thread.MIN_PRIORITY);

	    ORBInterface.addPreORBShutdown(new ReaperCleanup(reaperThread));
	    
	    reaperThread.start();
	}

	return OTS_TransactionReaper._theReaper;
    }

public static synchronized OTS_TransactionReaper create ()
    {
	return create(OTS_TransactionReaper.defaultCheckPeriod);
    }
    
public static synchronized OTS_TransactionReaper transactionReaper ()
    {
	return _theReaper;
    }
    
public static final long defaultCheckPeriod = 120000;  // in milliseconds
    
private OrderedList _list;
private long _checkPeriod;
    
private static OTS_TransactionReaper _theReaper = null;
private static ReaperThread reaperThread = null;
private static boolean _dynamic = false;
    
};

class ReaperThread extends Thread
{

public ReaperThread (OTS_TransactionReaper arg)
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

private OTS_TransactionReaper reaperObject;
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
