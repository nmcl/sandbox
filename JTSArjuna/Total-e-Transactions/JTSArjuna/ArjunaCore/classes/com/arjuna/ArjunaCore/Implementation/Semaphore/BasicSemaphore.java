/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: BasicSemaphore.java,v 1.1.4.1.2.1.8.1 2001/01/24 13:42:17 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Implementation.Semaphore;

import com.arjuna.ArjunaCore.ArjunaCoreNames;
import com.arjuna.ArjunaCore.Implementation.SemaphoreImple;
import com.arjuna.ArjunaCore.Interface.Semaphore;
import com.arjuna.ArjunaCommon.Template.KeyedList;
import com.arjuna.ArjunaCommon.Common.*;
import java.util.Hashtable;

import java.lang.InterruptedException;

/**
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

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS,
						 VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_GENERAL, "BasicSemapore::BasicSemaphore ( "+key+" )");
	}
    }

public void finalize ()
    {
	if (waiters.size() != 0)
	    ErrorStream.stream(ErrorStream.WARNING).println("BasicSemaphore being destroyed with waiters.");

	owner = null;
	waiters = null;
    }

    /**
     * Classic semaphore operations.
     */
    
public int lock ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS,
						 VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_GENERAL, "BasicSemapore::lock()");
	}

	synchronized (this)
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
	    }

	return Semaphore.SM_LOCKED;
    }
	    
public int unlock ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS,
						 VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_GENERAL, "BasicSemapore::unlock()");
	}

	synchronized (this)
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
		}
	    }

	return Semaphore.SM_UNLOCKED;
    }
    
public int tryLock ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS,
						 VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_GENERAL, "BasicSemapore::tryLock()");
	}

	synchronized (this)
	    {
		if ((owner == null) || (owner == Thread.currentThread()))
		    return this.lock();
		else
		    return Semaphore.SM_WOULD_BLOCK;
	    }
    }

public ClassName className ()
    {
	return ArjunaCoreNames.Implementation_Semaphore_BasicSemaphore();
    }
    
public static ClassName name ()
    {
	return ArjunaCoreNames.Implementation_Semaphore_BasicSemaphore();	
    }
    
    /**
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
    
private Thread    owner;
private int       useCount;
private Hashtable waiters;
private int       numberOfResources;
private String    semKey;

private static KeyedList semaphoreList = new KeyedList();
    
}

