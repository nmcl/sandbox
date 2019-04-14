/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: TransactionReaper.java,v 1.3 2003/08/11 15:04:14 nmcl Exp $
 */

package com.arjuna.ats.arjuna.coordinator;

import com.arjuna.ats.arjuna.common.Environment;
import com.arjuna.ats.arjuna.common.arjPropertyManager;
import com.arjuna.common.util.propertyservice.PropertyManager;
import com.arjuna.ats.arjuna.coordinator.Reapable;
import com.arjuna.ats.arjuna.coordinator.ActionStatus;

import com.arjuna.ats.internal.arjuna.template.OrderedList;
import com.arjuna.ats.internal.arjuna.template.OrderedListIterator;
import com.arjuna.ats.internal.arjuna.coordinator.*;

import com.arjuna.ats.arjuna.logging.tsLogger;
import com.arjuna.ats.arjuna.logging.FacilityCode;

import com.arjuna.common.util.logging.*;

/**
 * Class to record transactions with non-zero timeout values, and
 * class to implement a transaction reaper thread which terminates
 * these transactions once their timeout elapses.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: TransactionReaper.java,v 1.3 2003/08/11 15:04:14 nmcl Exp $
 * @since JTS 1.0.
 *
 *
 * @message com.arjuna.ats.arjuna.coordinator.TransactionReaper_1 [com.arjuna.ats.arjuna.coordinator.TransactionReaper_1] - TransactionReaper - could not create transaction list. Out of memory.
 * @message com.arjuna.ats.arjuna.coordinator.TransactionReaper_2 [com.arjuna.ats.arjuna.coordinator.TransactionReaper_2] - TransactionReaper::check - comparing {0}
 * @message com.arjuna.ats.arjuna.coordinator.TransactionReaper_3 [com.arjuna.ats.arjuna.coordinator.TransactionReaper_3] - TransactionReaper::check - rollback for {0}
 * @message com.arjuna.ats.arjuna.coordinator.TransactionReaper_4 [com.arjuna.ats.arjuna.coordinator.TransactionReaper_4] - TransactionReaper failed to force rollback on {0}
 * @message com.arjuna.ats.arjuna.coordinator.TransactionReaper_5 [com.arjuna.ats.arjuna.coordinator.TransactionReaper_5] - TransactionReaper failed to force rollback_only on {0}
 * @message com.arjuna.ats.arjuna.coordinator.TransactionReaper_6 [com.arjuna.ats.arjuna.coordinator.TransactionReaper_6] - TransactionReaper::getTimeout for {0} returning {1}
 */

public class TransactionReaper
{

    public static final String NORMAL = "NORMAL";
    public static final String DYNAMIC = "DYNAMIC";

    public TransactionReaper (long checkPeriod)
    {
	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
				     FacilityCode.FAC_ATOMIC_ACTION, 
				     "TransactionReaper::TransactionReaper ( "+checkPeriod+" )");
	}

	_list = new OrderedList();
	_checkPeriod = checkPeriod;

	if (_list == null)
	{
	    if (tsLogger.arjLoggerI18N.isFatalEnabled())
	    {
		tsLogger.arjLoggerI18N.fatal("com.arjuna.ats.arjuna.coordinator.TransactionReaper_1");
	    }
	    
	    throw new OutOfMemoryError();
	}
    }

    public void finalize ()
    {
	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.DESTRUCTORS, VisibilityLevel.VIS_PUBLIC,
				     FacilityCode.FAC_ATOMIC_ACTION, "TransactionReaper.finalize ()");
	}
	
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
	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
				     FacilityCode.FAC_ATOMIC_ACTION, "TransactionReaper::check ( "+timeout+" )");
	}
	
	if (_list == null)
	    return true;
	
	OrderedListIterator iter = new OrderedListIterator(_list);
	ReaperElement e = null;

	while ((e = (ReaperElement) iter.iterate()) != null)
	{
	    if (tsLogger.arjLoggerI18N.debugAllowed())
	    {
		tsLogger.arjLoggerI18N.debug(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					     FacilityCode.FAC_ATOMIC_ACTION, 
					     "com.arjuna.ats.arjuna.coordinator.TransactionReaper_2", 
					     new Object[]{Long.toString(e._absoluteTimeout)});
	    }

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
	    
	    if (e._control.running())
	    {
		/*
		 * If this is a local transaction, then we can roll it
		 * back completely. Otherwise, just mark it as rollback only.
		 */

		boolean problem = false;
		
		try
		{
		    if (e._control.cancel() == ActionStatus.ABORTED)
		    {
			if (tsLogger.arjLoggerI18N.debugAllowed())
			{
			    tsLogger.arjLoggerI18N.debug(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
							 FacilityCode.FAC_ATOMIC_ACTION, 
							 "com.arjuna.ats.arjuna.coordinator.TransactionReaper_3", 
							 new Object[]{e._control.get_uid()});
			}
		    }
		    else
			problem = true;
		}
		catch (Exception ex2)
		{
		    if (tsLogger.arjLoggerI18N.isWarnEnabled())
		    {
			tsLogger.arjLoggerI18N.warn("com.arjuna.ats.arjuna.coordinator.TransactionReaper_4", 
						    new Object[]{e._control});
		    }

		    problem = true;
		}
		
		if (problem)
		{
		    boolean error = false;
		    boolean printDebug = tsLogger.arjLoggerI18N.isWarnEnabled();
		     
		    try
		    {
			error = !e._control.preventCommit();
		    }
		    catch (Exception ex3)
		    {
			error = true;
		    }
		     
		    if (error || printDebug)
		    {
			if (error)
			{
			    if (tsLogger.arjLoggerI18N.isWarnEnabled())
			    {
				tsLogger.arjLoggerI18N.warn("com.arjuna.ats.arjuna.coordinator.TransactionReaper_5", new Object[]{e._control});
			    }
			}
			else
			{
			    if (tsLogger.arjLoggerI18N.debugAllowed())
			    {
				tsLogger.arjLoggerI18N.debug(DebugLevel.FUNCTIONS, 
							     VisibilityLevel.VIS_PUBLIC,
							     FacilityCode.FAC_ATOMIC_ACTION, 
							     "com.arjuna.ats.arjuna.coordinator.TransactionReaper_3", new Object[]{e._control});
			    }
			}
		    }
		}
	    }
	    
	    _list.remove(e);
	}
	
	System.gc();  // do some garbage collection while we're at it!
	
	return true;
    }
    
    /**
     * @return the number of items in the reaper's list.
     * @since JTS 2.2.
     */

    public final synchronized long numberOfTransactions ()
    {
	return ((_list == null) ? 0 : _list.size());
    }
    
    /**
     * timeout is given in seconds, but we work in milliseconds.
     */
 
    public final synchronized boolean insert (Reapable control, int timeout)
    {
	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC, 
				     FacilityCode.FAC_ATOMIC_ACTION, 
				     "TransactionReaper::insert ( "+control+", "+timeout+" )");
	}
	
	/*
	 * Ignore if the timeout is zero, since this means the transaction
	 * should never timeout.
	 */

	if (timeout == 0)
	    return true;
    
	TransactionReaper._lifetime += timeout;
	
	/*
	 * If the timeout for this transaction is less than the
	 * current timeout for the reaper thread (or one is not set for
	 * the reaper thread) then use that timeout and interrupt the thread
	 * to get it to recheck.
	 */

	if ((timeout < _checkPeriod) || (_checkPeriod == Long.MAX_VALUE))
	{
	    _checkPeriod = timeout*1000;  // convert to milliseconds!
	    TransactionReaper._reaperThread.interrupt();
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

    public final synchronized boolean remove (java.lang.Object control)
    {
	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
				     FacilityCode.FAC_ATOMIC_ACTION, "TransactionReaper::remove ( "+control+" )");
	}
	
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

    public final synchronized int getTimeout (Object control)
    {
	if ((_list == null) || (control == null))
	{
	    if (tsLogger.arjLogger.debugAllowed())
	    {
		tsLogger.arjLogger.debug(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					 FacilityCode.FAC_ATOMIC_ACTION, 
					 "TransactionReaper::getTimeout for "+control+" returning 0");
	    }
	    
	    return 0;
	}
    
	ReaperElement e = null;
	OrderedListIterator iter = new OrderedListIterator(_list);
	
	while ((e = (ReaperElement) iter.iterate()) != null)
	{
	    try
	    {
		if (e._control.equals(control))
		{
		    iter = null;

		    if (tsLogger.arjLoggerI18N.debugAllowed())
		    {
			tsLogger.arjLoggerI18N.debug(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						     FacilityCode.FAC_ATOMIC_ACTION, 
						     "com.arjuna.ats.arjuna.coordinator.TransactionReaper_6", 
						     new Object[]{control, Integer.toString(e._timeout)});
		    }

		    return e._timeout;
		}
	    }
	    catch (Exception e2)
	    {
		break;
	    }
	}

	if (tsLogger.arjLoggerI18N.debugAllowed())
	{
	    tsLogger.arjLoggerI18N.debug(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
				     FacilityCode.FAC_ATOMIC_ACTION, 
				     "com.arjuna.ats.arjuna.coordinator.TransactionReaper_6", 
				     new Object[]{control, "0"});
	}

	return 0;
    }

    /**
     * Currently we let the reaper thread run at same priority as other
     * threads. Could get priority from environment.
     */

    public static synchronized TransactionReaper create (long checkPeriod)
    {
	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
				     FacilityCode.FAC_ATOMIC_ACTION, "TransactionReaper::create ( "+checkPeriod+" )");
	}
	
	if (TransactionReaper._theReaper == null)
	{
	    String mode = arjPropertyManager.propertyManager.getProperty(Environment.TX_REAPER_MODE);
	    
	    if (mode != null)
	    {
		if (mode.compareTo(TransactionReaper.DYNAMIC) == 0)
		    TransactionReaper._dynamic = true;
	    }
	    
	    if (!TransactionReaper._dynamic)
	    {
		String timeoutEnv = arjPropertyManager.propertyManager.getProperty(Environment.TX_REAPER_TIMEOUT);

		if (timeoutEnv != null)
		{
		    Long l = null;
		
		    try
		    {
			l = new Long(timeoutEnv);
			checkPeriod = l.longValue();
			
			l = null;
		    }
		    catch (NumberFormatException e)
		    {
			tsLogger.arjLogger.warn("TransactionReaper::create - "+e);
		    }
		}
	    }
	    else
		checkPeriod = Long.MAX_VALUE;
		
	    TransactionReaper._theReaper = new TransactionReaper(checkPeriod);
	    
	    _reaperThread = new ReaperThread(TransactionReaper._theReaper);
	    //	    _reaperThread.setPriority(Thread.MIN_PRIORITY);

	    _reaperThread.setDaemon(true);
	    
	    _reaperThread.start();
	}

	return TransactionReaper._theReaper;
    }

    public static synchronized TransactionReaper create ()
    {
	return create(TransactionReaper.defaultCheckPeriod);
    }
    
    public static synchronized TransactionReaper transactionReaper ()
    {
	return transactionReaper(false);
    }

    /*
     * If parameter is true then do a create.
     */

    public static synchronized TransactionReaper transactionReaper (boolean createReaper)
    {
	if (createReaper)
	    return create();
	else
	    return _theReaper;
    }
    
    /*
     * Don't bother synchronizing as this is only an estimate anyway.
     */

    public static final long transactionLifetime ()
    {
	return TransactionReaper._lifetime;
    }

    public static final long defaultCheckPeriod = 120000;  // in milliseconds

    static final void reset ()
    {
	_theReaper = null;
    }
    
    private OrderedList _list;
    private long        _checkPeriod;
    
    private static TransactionReaper _theReaper = null;
    private static ReaperThread      _reaperThread = null;
    private static boolean           _dynamic = false;
    private static long              _lifetime = 0;
 
}



