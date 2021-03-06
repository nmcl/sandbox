/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: BasicSemaphore.java,v 1.3 2003/08/11 15:04:48 nmcl Exp $
 */

package com.arjuna.ats.internal.txoj.semaphore;

import com.arjuna.ats.txoj.TxOJNames;
import com.arjuna.ats.txoj.semaphore.*;
import com.arjuna.ats.internal.arjuna.template.KeyedList;
import com.arjuna.ats.txoj.common.*;
import com.arjuna.ats.arjuna.gandiva.ClassName;

import com.arjuna.ats.txoj.logging.txojLogger;
import com.arjuna.ats.txoj.logging.FacilityCode;

import com.arjuna.common.util.logging.*;

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

	if (txojLogger.aitLogger.isDebugEnabled())
	{
	    txojLogger.aitLogger.debug(DebugLevel.CONSTRUCTORS,
				     VisibilityLevel.VIS_PUBLIC,
				       com.arjuna.ats.arjuna.logging.FacilityCode.FAC_GENERAL, "BasicSemapore::BasicSemaphore ( "+key+" )");
	}
    }

    /**
     * @message com.arjuna.ats.internal.txoj.semaphore.BasicSemaphore_1 [com.arjuna.ats.internal.txoj.semaphore.BasicSemaphore_1] - BasicSemaphore being destroyed with waiters.
     */
public void finalize ()
    {
	if (waiters.size() != 0)
	{
	    if (txojLogger.aitLoggerI18N.isWarnEnabled())
	    {
		txojLogger.aitLoggerI18N.warn("com.arjuna.ats.internal.txoj.semaphore.BasicSemaphore_1");
	    }
	}

	owner = null;
	waiters = null;
    }

    /**
     * Classic semaphore operations.
     */
    
public int lock ()
    {
	if (txojLogger.aitLogger.isDebugEnabled())
	{
	    txojLogger.aitLogger.debug(DebugLevel.FUNCTIONS,
				     VisibilityLevel.VIS_PUBLIC,
				       com.arjuna.ats.arjuna.logging.FacilityCode.FAC_GENERAL, "BasicSemapore::lock()");
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
	if (txojLogger.aitLogger.isDebugEnabled())
	{
	    txojLogger.aitLogger.debug(DebugLevel.FUNCTIONS,
				     VisibilityLevel.VIS_PUBLIC,
				       com.arjuna.ats.arjuna.logging.FacilityCode.FAC_GENERAL, "BasicSemapore::unlock()");
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
	if (txojLogger.aitLogger.isDebugEnabled())
	{
	    txojLogger.aitLogger.debug(DebugLevel.FUNCTIONS,
				     VisibilityLevel.VIS_PUBLIC,
				       com.arjuna.ats.arjuna.logging.FacilityCode.FAC_GENERAL, "BasicSemapore::tryLock()");
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
	return TxOJNames.Implementation_Semaphore_BasicSemaphore();
    }
    
public static ClassName name ()
    {
	return TxOJNames.Implementation_Semaphore_BasicSemaphore();	
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

