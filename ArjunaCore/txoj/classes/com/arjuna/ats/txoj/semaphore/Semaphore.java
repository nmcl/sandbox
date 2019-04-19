/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: Semaphore.java,v 1.1 2003/06/19 11:56:14 nmcl Exp $
 */

package com.arjuna.ats.txoj.semaphore;

import com.arjuna.ats.txoj.TxOJNames;
import com.arjuna.ats.txoj.common.*;
import com.arjuna.ats.internal.txoj.Implementations;
import com.arjuna.ats.arjuna.gandiva.inventory.Inventory;
import com.arjuna.ats.arjuna.gandiva.ClassName;
import com.arjuna.ats.arjuna.gandiva.ObjectName;
import com.arjuna.ats.arjuna.common.Uid;

import java.io.IOException;

/**
 * Semaphore interface. Binds to interpreter specific implementation.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: Semaphore.java,v 1.1 2003/06/19 11:56:14 nmcl Exp $
 * @since JTS 1.0.
 */

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
	
	Object ptr = Inventory.inventory().createResources(TxOJNames.Implementation_Semaphore_BasicSemaphore(), param);

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
    
}


