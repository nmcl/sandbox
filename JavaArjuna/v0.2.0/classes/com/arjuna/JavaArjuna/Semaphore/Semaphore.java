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

package com.arjuna.JavaArjuna.Semaphore;

import com.arjuna.JavaArjuna.Semaphore.SemaphoreImple;
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
	/*
	 * In the original JavaArjuna there was only one Semaphore implementation.
	 */

	_imple = new BasicSemaphore(key);
    }

    /*
     * https://github.com/nmcl/sandbox/issues/75
public void finalize ()
    {
	_imple = null;
    }
    */

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

public String className ()
    {
	return ((_imple != null) ? _imple.className() : null);
    }
    
private SemaphoreImple _imple;
    
};


