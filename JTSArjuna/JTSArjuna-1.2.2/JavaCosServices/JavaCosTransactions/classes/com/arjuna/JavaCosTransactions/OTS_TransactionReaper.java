/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OTS_TransactionReaper.java,v 1.10.2.1 1999/02/05 09:57:37 nmcl Exp $
 */

package com.arjuna.JavaCosTransactions;

import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.OrbCommon.ORBCleanup;
import com.arjuna.JavaArjunaLite.Atomic.BasicAction;
import com.arjuna.JavaArjunaLite.Atomic.ActionStatus;
import com.arjuna.JavaGandiva.Common.Uid;
import com.arjuna.JavaGandiva.Template.*;
import org.omg.CosTransactions.*;
import com.arjuna.ArjunaOTS.*;

import com.arjuna.ArjunaOTS.ActiveThreads;
import com.arjuna.ArjunaOTS.ActiveTransaction;
import com.arjuna.ArjunaOTS.BadControl;
import com.arjuna.ArjunaOTS.Destroyed;

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

public ReaperElement (Control tran, int timeout)
    {
	/*
	 * Given a timeout period in microseconds, calculate its absolute value
	 * from the current time of day.
	 */
	
	_control = tran;
	_absoluteTimeout = timeout + System.currentTimeMillis()*1000;
	_timeout = timeout;
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

protected Control _control;
protected long _absoluteTimeout;
protected int _timeout;
    
};


public class OTS_TransactionReaper
{

public OTS_TransactionReaper (long checkPeriod)
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

    /*
     * Only check for one at a time to prevent starvation.
     */
    
public final boolean check (long timeout)
    {
	OrderedListIterator iter = new OrderedListIterator(_list);
	ReaperElement e = null;

	while ((e = (ReaperElement) iter.iterate()) != null)
	{
	    if (timeout*1000 >= e._absoluteTimeout)  // remember to convert to microseconds
		break;
	    else
		return true;
	}

	if (e != null)
	{
	    /*
	     * Currently we lazily determine that transactions may have
	     * previously terminated but may still be on our list.
	     */

	    Coordinator coord = null;
	    org.omg.CosTransactions.Status stat = org.omg.CosTransactions.Status.StatusActive;
	    String transactionName = null;

	    try
	    {
		coord = e._control.get_coordinator();
		stat = coord.get_status();
	    }
	    catch (Exception ex1)
	    {
		coord = null;

		return false;
	    }

	    try
	    {
		transactionName = coord.get_transaction_name();
	    }
	    catch (Exception ex2)
	    {
		transactionName = "**UNKNOWN**";
	    }
	    
	    /*
	     * Only force rollback if the transaction is still running.
	     */
	    
	    if (stat == org.omg.CosTransactions.Status.StatusActive)
	    {
		/*
		 * If this is a local transaction, then we can roll it
		 * back completely. Otherwise, just mark it as rollback only.
		 */

		OTS_Control lCont = OTS_Utility.localControl(e._control);

		if (lCont != null)
		{
		    OTS_Transaction transaction = lCont.getImplHandle();

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
		    try
		    {
			coord.rollback_only();
		    }
		    catch (Exception ex3)
		    {
			System.err.println("WARNING: transaction reaper failed to rollback_only "+transactionName);
		    }
		}

		coord = null;
	    }
		
	    _list.remove(e);

	    /*
	     * Although we have terminated the transaction we do not
	     * destroy the control since other threads may still have
	     * references to it and we want them to know what happened.
	     */

	    e._control = null;
	    e = null;
	}

	System.gc();  // do some garbage collection while we're at it!
	
	return true;
    }
    
    /*
     * timeout is given in seconds, but we work in microseconds.
     */
 
public final synchronized boolean insert (Control control, int timeout)
    {
	/*
	 * Ignore if the timeout is zero, since this means the transaction
	 * should never timeout.
	 */

	if (timeout == 0)
	    return true;
    
	ReaperElement e = new ReaperElement(control, timeout*1000000);

	if ((_list != null) && _list.insert(e))
	    return true;
	else
	{
	    e = null;
	    return false;
	}
    }

public final synchronized boolean remove (Control control)
    {
	if ((_list == null) || (control == null))
	    return false;
    
	ReaperElement e = null;
	OrderedListIterator iter = new OrderedListIterator(_list);
	boolean result = false;
	Coordinator removeCoord;

	try
	{
	    removeCoord = control.get_coordinator();
	}
	catch (Exception e1)
	{
	    removeCoord = null;
	}

	if (removeCoord == null)
	{
	    System.err.println("WARNING: transaction reaper could not remove transaction - null coordinator.");
	    return false;
	}

	boolean found = false;
	Coordinator listCoord;
	
	while (((e = (ReaperElement) iter.iterate()) != null) && !found)
	{
	    listCoord = null;

	    try
	    {
		listCoord = e._control.get_coordinator();

		if (listCoord.is_same_transaction(removeCoord))
		{
		    found = true;
		    break;
		}
	    }
	    catch (Exception e2)
	    {
		listCoord = null;

		break;
	    }
	}

	listCoord = null;
	removeCoord = null;

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
     * Return in seconds!
     */
    
public final synchronized int getTimeout (Control control)
    {
	if ((_list == null) || (control == null))
	    return 0;
    
	ReaperElement e = null;
	OrderedListIterator iter = new OrderedListIterator(_list);
	Coordinator theCoord = null;

	try
	{
	    theCoord = control.get_coordinator();
	}
	catch (Exception e1)
	{
	    theCoord = null;
	}

	if (theCoord == null)
	{
	    System.err.println("WARNING: transaction reaper could not get transaction coordinator for timeout.");
	    return 0;
	}

	Coordinator listCoord = null;
	
	while ((e = (ReaperElement) iter.iterate()) != null)
	{
	    listCoord = null;

	    try
	    {
		listCoord = e._control.get_coordinator();

		if (listCoord.is_same_transaction(theCoord))
		{
		    return e._timeout;
		}
	    }
	    catch (Exception e2)
	    {
		listCoord = null;

		break;
	    }
	}

	listCoord = null;
	theCoord = null;
	iter = null;

	return 0;
    }
    
    /*
     * Currently we let the reaper thread run at same priority as other threads.
     */

public static synchronized OTS_TransactionReaper create (long checkPeriod)
    {
	if (OTS_TransactionReaper._theReaper == null)
	{
	    OTS_TransactionReaper._theReaper = new OTS_TransactionReaper(checkPeriod);
	    
	    reaperThread = new ReaperThread(OTS_TransactionReaper._theReaper);
	    reaperThread.setPriority(Thread.MIN_PRIORITY);

	    ORBInterface.addORBCleanup(new ReaperCleanup(reaperThread));
	    
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
    
public static final long defaultCheckPeriod = 1000000000;  // in microseconds.
    
private OrderedList _list;
private long _checkPeriod;
    
private static OTS_TransactionReaper _theReaper = null;
private static Thread reaperThread = null;
    
};

class ReaperThread extends Thread
{

public ReaperThread (OTS_TransactionReaper arg)
    {
	reaperObject = arg;
	sleepPeriod = arg.checkingPeriod();
    }
    
public void run ()
    {
	for (;;)
	{
	    /*
	     * Cannot assume we sleep for the entire period. We may
	     * be interrupted.
	     */

	    try
	    {
		int nanos = ((int) sleepPeriod%1000)*1000;

		Thread.sleep(sleepPeriod/1000000, nanos);
	    }
	    catch (Exception e)
	    {
	    }

	    reaperObject.check(System.currentTimeMillis());
	}
    }

private OTS_TransactionReaper reaperObject;
private long sleepPeriod;

};


/*
 * A Java application will not cleanly exit it a thread
 * is still running, so we must explicitly kill the terminating
 * thread. We could get it to terminate any outstanding transactions
 * as well. (Only those with non-zero timeouts!) We don't at the
 * moment, since some of these could be remote.
 */

class ReaperCleanup implements ORBCleanup
{

public ReaperCleanup (Thread thread)
    {
	_reaperThread = thread;
    }

public void cleanup ()
    {
	if (_reaperThread != null)
	{
	    _reaperThread.stop();
	}
    }

private Thread _reaperThread;
    
};
