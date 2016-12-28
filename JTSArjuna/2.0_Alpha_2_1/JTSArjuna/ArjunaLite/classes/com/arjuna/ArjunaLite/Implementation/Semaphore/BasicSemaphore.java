/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: BasicSemaphore.java,v 1.1 2000/02/25 14:04:14 nmcl Exp $
 */

package com.arjuna.ArjunaLite.Implementation.Semaphore;

import com.arjuna.ArjunaLite.ArjunaLiteNames;
import com.arjuna.ArjunaLite.Implementation.SemaphoreImple;
import com.arjuna.ArjunaLite.Interface.Semaphore;
import com.arjuna.ArjunaCommon.Template.KeyedList;
import com.arjuna.ArjunaCommon.Common.*;
import java.util.Hashtable;

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
	waiters = new Hashtable();
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
		waiters.put(t, t);

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
	return ArjunaLiteNames.Implementation_Semaphore_BasicSemaphore();
    }
    
public static ClassName name ()
    {
	return ArjunaLiteNames.Implementation_Semaphore_BasicSemaphore();	
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
private Hashtable waiters;
private int numberOfResources;
private String semKey;

private static KeyedList semaphoreList = new KeyedList();
    
}

