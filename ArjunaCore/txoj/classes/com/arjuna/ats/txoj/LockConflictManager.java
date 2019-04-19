/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: LockConflictManager.java,v 1.1 2003/06/19 11:56:10 nmcl Exp $
 */

package com.arjuna.ats.txoj;

import java.util.Calendar;
import java.util.Date;

import java.lang.InterruptedException;

/**
 * An instance of this class is used to determine what to do in the
 * event of a lock conflict for a given object. If the timeout and
 * retry values are >=0 then we use them to sleep the thread which tried
 * to get the lock. If the retry value is -100 (LockManager.waitTotalTimeout)
 * then the thread will block for up to the total timeout and be signalled
 * either when the timeout occurs, or when the lock is actually released.
 */

class LockConflictManager
{

LockConflictManager ()
    {
	_lock = new Object();
	_signals = 0;
    }

    /**
     * Wait for the specified timeout and retry. We may either sleep the
     * thread, or block it on a mutex.
     *
     * Returns the time taken to wait.
     */

int wait (int retry, int waitTime)
    {
	/*
	 * If the retry is -1 then we wait on the object as if it
	 * were a lock. Otherwise we do the usual sleep call.
	 */

	if (retry < 0)
	{
	    /*
	     * Wait for the lock object to be signalled.
	     */

	    Date d1 = Calendar.getInstance().getTime();
	    
	    synchronized (_lock)
		{
		    try
		    {
			/*
			 * Consume an old signal. May cause us to go round
			 * the loop quicker than we should, but its better
			 * than synchronizing signal and wait.
			 */

			if (_signals == 0)
			{
			    _lock.wait(waitTime);
			}
			else
			{
			    _signals--;

			    return waitTime;
			}
		    }
		    catch (InterruptedException e)
		    {
		    }
		}

	    Date d2 = Calendar.getInstance().getTime();

	    return (int) (d2.getTime() - d1.getTime());
	}
	else
	{
	    try
	    {
		/* hope things happen in time */
	    
		Thread.sleep(waitTime);
	    }
	    catch (InterruptedException e)
	    {
	    }

	    return 0;
	}
    }    

    /**
     * Signal that the lock has been released.
     */
    
void signal ()
    {
	synchronized (_lock)
	    {
		_lock.notifyAll();

		_signals++;

		if (_signals < 0)  // check for overflow
		    _signals = 1;
	    }
    }
    
private Object _lock;
private int    _signals;
    
}
