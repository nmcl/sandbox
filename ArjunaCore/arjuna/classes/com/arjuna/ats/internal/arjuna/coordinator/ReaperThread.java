/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ReaperThread.java,v 1.3 2003/08/11 15:04:17 nmcl Exp $
 */

package com.arjuna.ats.internal.arjuna.coordinator;

import com.arjuna.ats.arjuna.logging.tsLogger;
import com.arjuna.ats.arjuna.logging.FacilityCode;

import com.arjuna.ats.arjuna.coordinator.BasicAction;
import com.arjuna.ats.arjuna.coordinator.ActionStatus;
import com.arjuna.ats.arjuna.coordinator.TransactionReaper;

import com.arjuna.common.util.logging.*;

/**
 * Class to record transactions with non-zero timeout values, and
 * class to implement a transaction reaper thread which terminates
 * these transactions once their timeout elapses.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ReaperThread.java,v 1.3 2003/08/11 15:04:17 nmcl Exp $
 * @since JTS 1.0.
 */

public class ReaperThread extends Thread
{

public ReaperThread (TransactionReaper arg)
    {
	reaperObject = arg;
	sleepPeriod = reaperObject.checkingPeriod();
	_shutdown = false;

	this.setDaemon(true);
    }
    
    /**
     * @message com.arjuna.ats.internal.arjuna.coordinator.ReaperThread_1 [com.arjuna.ats.internal.arjuna.coordinator.ReaperThread_1] - Thread {0} sleeping for {1}
     */

public void run ()
    {
	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
				     FacilityCode.FAC_ATOMIC_ACTION, "ReaperThread.run ()");
	}
	
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
		sleepPeriod = reaperObject.checkingPeriod();

		long oldPeriod = sleepPeriod;
		long beforeTime = System.currentTimeMillis();

		try
		{
		    if (tsLogger.arjLoggerI18N.debugAllowed())
		    {
			tsLogger.arjLoggerI18N.debug(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						     FacilityCode.FAC_ATOMIC_ACTION,
						     "com.arjuna.ats.internal.arjuna.coordinator.ReaperThread_1", 
						     new Object[]{Thread.currentThread(),
								  Long.toString(sleepPeriod)});
		    }

		    Thread.sleep(sleepPeriod);

		    done = true;
		}
		catch (InterruptedException e1)
		{
		    /*
		     * Has timeout been changed?
		     */

		    if (reaperObject.checkingPeriod() != oldPeriod)
		    {
			done = true;
		    }
		    else
		    {
			long afterTime = System.currentTimeMillis();

			if (afterTime - beforeTime < reaperObject.checkingPeriod())
			{
			    done = true;
			}
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

	    if (reaperObject.numberOfTransactions() == 0)
	    {
		sleepPeriod = Long.MAX_VALUE;
	    }
	}
    }

    public void shutdown ()
    {
	_shutdown = true;
    }

    private TransactionReaper reaperObject;
    private long              sleepPeriod;
    private boolean           _shutdown;

    
}

