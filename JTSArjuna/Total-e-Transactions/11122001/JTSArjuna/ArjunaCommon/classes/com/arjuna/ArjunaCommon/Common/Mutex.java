/*
 * Copyright (C) 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Mutex.java,v 1.1.2.1.20.2 2001/04/23 11:45:24 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Common;

import java.lang.InterruptedException;

/*
 * This is a reentrant Mutex implementation.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: Mutex.java,v 1.1.2.1.20.2 2001/04/23 11:45:24 nmcl Exp $
 * @since JTS 2.1.
 */

public class Mutex
{

public static final int LOCKED = 0;
public static final int UNLOCKED = 1;
public static final int WOULD_BLOCK = 2;
public static final int ERROR = 3;

public Mutex ()
    {
	//	this(true);
	this(false);
    }

public Mutex (boolean reentrant)
    {
	_lock = new Object();
	_users = 0;
	_owner = null;
	_reentrant = reentrant;
	_recursiveCalls = 0;
    }
    
public void finalize ()
    {
	if (_users != 0)
	    ErrorStream.warning().println("Mutex being destroyed with waiters.");

	synchronized (_lock)
	    {
		_lock.notifyAll();
	    }
    }

    /**
     * Classic mutex operations.
     */
    
public int lock ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS,
						 VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_GENERAL, "Mutex::lock()");
	}

	synchronized (_lock)
	    {
		if (_users == 0)
		{
		    _users = 1;

		    if (_reentrant)
		    {
			_owner = Thread.currentThread();
			_recursiveCalls = 1;
		    }
		}
		else
		{
		    boolean done = false;
		    
		    if (_reentrant)
		    {
			if (_owner == Thread.currentThread())
			{
			    _recursiveCalls++;

			    done = true;
			}
		    }

		    if (!done)
		    {
			_users++;
		    
			try
			{
			    _lock.wait();
			}
			catch (InterruptedException e)
			{
			}
		    }
		}
	    }

	return Mutex.LOCKED;
    }
	    
public int unlock ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS,
						 VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_GENERAL, "Mutex::unlock()");
	}

	synchronized (_lock)
	    {
		if (_users <= 0)
		{
		    ErrorStream.warning().println("Mutex.unlock - called by non-owning thread!");

		    return Mutex.LOCKED;
		}
		else
		{
		    boolean done = false;
		    
		    if (_reentrant)
		    {
			if (_owner == Thread.currentThread())
			{
			    if (--_recursiveCalls == 0)
				_owner = null;

			    done = true;
			}
			else
			{
			    ErrorStream.warning().println("Mutex.unlock - called by non-owning thread!");

			    return Mutex.LOCKED;
			}
		    }

		    if (!done)
		    {
			if (--_users >= 0)
			{
			    _lock.notify();
			}
		    }
		}
	    }

	return Mutex.UNLOCKED;
    }
    
public int tryLock ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS,
						 VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_GENERAL, "Mutex::tryLock()");
	}

	synchronized (_lock)
	    {
		if (_users == 0)
		    return lock();
		else
		{
		    if (_reentrant)
		    {
			if (_owner == Thread.currentThread())
			    return lock();
		    }

		    return Mutex.WOULD_BLOCK;
		}
	    }
    }

private Object  _lock;
private int     _users;
private Thread  _owner;
private boolean _reentrant;
private int     _recursiveCalls;
    
}

