/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ReaperThread.java,v 1.1 2002/11/25 10:51:46 nmcl Exp $
 */

package com.arjuna.mwlabs.wsas.activity;

/**
 * Class to record transactions with non-zero timeout values, and
 * class to implement a transaction reaper thread which terminates
 * these transactions once their timeout elapses.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ReaperThread.java,v 1.1 2002/11/25 10:51:46 nmcl Exp $
 * @since 1.0.
 */

public class ReaperThread extends Thread
{

    public ReaperThread (ActivityReaper arg)
    {
	_reaperObject = arg;
	_sleepPeriod = _reaperObject.checkingPeriod();
	_shutdown = false;

	this.setDaemon(true);
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
		_sleepPeriod = _reaperObject.checkingPeriod();

		long oldPeriod = _sleepPeriod;
		long beforeTime = System.currentTimeMillis();

		try
		{
		    Thread.sleep(_sleepPeriod);

		    done = true;
		}
		catch (InterruptedException e1)
		{
		    /*
		     * Has timeout been changed?
		     */

		    if (_reaperObject.checkingPeriod() != oldPeriod)
		    {
			done = true;
		    }
		    else
		    {
			long afterTime = System.currentTimeMillis();

			if (afterTime - beforeTime < _reaperObject.checkingPeriod())
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

	    _reaperObject.check(System.currentTimeMillis());

	    if (_reaperObject.numberOfActivities() == 0)
	    {
		_sleepPeriod = Long.MAX_VALUE;
	    }
	}
    }

    public void shutdown ()
    {
	_shutdown = true;
    }

    private ActivityReaper _reaperObject;
    private long           _sleepPeriod;
    private boolean        _shutdown;

    
}

