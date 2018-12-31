/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BasicSemaphore.java,v 1.6 1998/07/06 13:26:49 nmcl Exp $
 */

package com.arjuna.JavaArjuna.ClassLib;

import com.arjuna.JavaArjuna.Template.BasicList;
import com.arjuna.JavaArjuna.Template.KeyedList;
import com.arjuna.JavaArjuna.Common.*;

import java.lang.InterruptedException;

/*
 * Semaphore interface. Binds to interpreter specific implementation.
 */

/*
 * Actually a mutex at present since we assume resource count of 1.
 */

public class BasicSemaphore extends SemaphoreImple
{
    
public BasicSemaphore (String key)
    {
	owner = null;
	useCount = 0;
	waiters = new BasicList();
	numberOfResources = 1;
	semKey = key;
    }

public void finalize ()
    {
	if (waiters.size() != 0)
	    System.err.println("Error - BasicSemaphore being destroyed with waiters.");

	owner = null;
	waiters = null;
    }

    /* classic semaphore operations */
    
public synchronized int lock ()
    {
	Thread t = Thread.currentThread();
	
	if (owner == null)
	    owner = t;
	else
	{
	    if (owner != t)
	    {
		waiters.add(t);

		while (owner != null)
		{
		    try
		    {
			this.wait();
		    }
		    catch (InterruptedException e)
		    {
		    }
		}

		waiters.remove(t);
		
		owner = t;
	    }
	}

	useCount++;

	return Semaphore.SM_LOCKED;
    }
	    
public synchronized int unlock ()
    {
	Thread t = Thread.currentThread();
	
	if (owner != t)
	    return Semaphore.SM_ERROR;
	else
	{
	    if (--useCount == 0)
	    {
		owner = null;
		
		if (waiters.size() > 0)
		{
		    this.notify();
		}
	    }

	    return Semaphore.SM_UNLOCKED;
	}
    }
    
public synchronized int tryLock ()
    {
	if ((owner == null) || (owner == Thread.currentThread()))
	    return this.lock();
	else
	    return Semaphore.SM_WOULD_BLOCK;
    }

public ClassName className ()
    {
	return JavaArjunaLiteNames.Implementation_Semaphore_BasicSemaphore();
    }
    
public static ClassName name ()
    {
	return JavaArjunaLiteNames.Implementation_Semaphore_BasicSemaphore();	
    }
    
    /*
     * Create semaphore with value of 1.
     */
    
public static BasicSemaphore create (Object[] param)
    {
	if (param == null)
	    return null;
	
	String key = (String) param[0];
	Object ptr = semaphoreList.get(key);

	if (ptr != null)
	{
	    if (ptr instanceof BasicSemaphore)
		return (BasicSemaphore) ptr;
	    else
		return null;
	}

	/*
	 * Not in list, so create and add before returning.
	 */
	
	BasicSemaphore sem = new BasicSemaphore(key);

	semaphoreList.add(sem, key);
	
	return sem;
    }
    
private Thread owner;
private int useCount;
private BasicList waiters;
private int numberOfResources;
private String semKey;

private static KeyedList semaphoreList = new KeyedList();
    
}

