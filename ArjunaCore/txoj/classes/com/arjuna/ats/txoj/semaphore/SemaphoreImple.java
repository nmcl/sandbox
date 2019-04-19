/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: SemaphoreImple.java,v 1.1 2003/06/19 11:56:14 nmcl Exp $
 */

package com.arjuna.ats.txoj.semaphore;

import com.arjuna.ats.arjuna.gandiva.ClassName;

/**
 * Implementations of semaphore classes should derive from this
 * class and provide implementations of the relevant methods.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: SemaphoreImple.java,v 1.1 2003/06/19 11:56:14 nmcl Exp $
 * @since JTS 1.0.
 */

public abstract class SemaphoreImple
{

    /*
     * Classic semaphore operations.
     */

public abstract int lock ();
public abstract int unlock ();
public abstract int tryLock ();

public ClassName className ()
    {
	return SemaphoreImple._className;
    }

public static ClassName name ()
    {
	return SemaphoreImple._className;
    }

private static final ClassName _className = new ClassName("SemaphoreImple");
    
}
