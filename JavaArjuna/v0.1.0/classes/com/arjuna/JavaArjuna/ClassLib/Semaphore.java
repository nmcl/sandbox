/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Semaphore.java,v 1.5 1998/07/06 13:26:53 nmcl Exp $
 */

package com.arjuna.JavaArjuna.ClassLib;

import com.arjuna.JavaArjuna.ClassLib.SemaphoreImple;
import com.arjuna.JavaArjuna.Common.*;

import java.io.IOException;

public class Semaphore
{

public static final int SM_LOCKED = 0;
public static final int SM_UNLOCKED = 1;
public static final int SM_WOULD_BLOCK = 2;
public static final int SM_ERROR = 3;

    /**
     * Create a semaphore with the default implementation.
     */
    
public Semaphore (String key)
    {
	Object[] param = new Object[1];

	param[0] = key;
	
	Object ptr = Inventory.inventory().createResources(JavaArjunaLiteNames.Implementation_Semaphore_BasicSemaphore(), param);

	if (ptr instanceof SemaphoreImple)
	    _imple = (SemaphoreImple) ptr;
	else
	    _imple = null;
    }
    
public Semaphore (ClassName type)
    {
	Object ptr = Inventory.inventory().createVoid(type);

	if (ptr instanceof SemaphoreImple)
	    _imple = (SemaphoreImple) ptr;
	else
	    _imple = null;
    }

public Semaphore (ObjectName objName)
    {
	_imple = null;
	
	if (objName != null)
	{
	    try
	    {
		ClassName semaphoreType = objName.getClassNameAttribute("SemaphoreType");
		ObjectName semaphoreName = objName.getObjectNameAttribute("SemaphoreName");
		
		Object ptr = Inventory.inventory().createObjectName(semaphoreType, semaphoreName);

		if (ptr instanceof SemaphoreImple)
		    _imple = (SemaphoreImple) ptr;
	    }
	    catch (IOException e)
	    {
	    }
	}
    }    

public void finalize ()
    {
	_imple = null;
    }

public int lock ()
    {
	return ((_imple != null) ? _imple.lock() : Semaphore.SM_ERROR);
    }
    
public int unlock ()
    {
	return ((_imple != null) ? _imple.unlock() : Semaphore.SM_ERROR);
    }
    
public int tryLock ()
    {
	return ((_imple != null) ? _imple.tryLock() : Semaphore.SM_ERROR);
    }
    
public ClassName className ()
    {
	return ((_imple != null) ? _imple.className() : ClassName.invalid());
    }
    
private SemaphoreImple _imple;

    static 
    {
	if (!Implementations.added())
	    Implementations.initialise();
    }
    
};


