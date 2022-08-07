/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: SemaphoreImple.java,v 1.1.4.1.2.1.4.1 2000/12/21 11:21:09 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Implementation;

import com.arjuna.ArjunaCommon.Common.ClassName;

/**
 * Implementations of semaphore classes should derive from this
 * class and provide implementations of the relevant methods.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: SemaphoreImple.java,v 1.1.4.1.2.1.4.1 2000/12/21 11:21:09 nmcl Exp $
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
