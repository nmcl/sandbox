/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: SemaphoreImple.java,v 1.3 1998/07/06 13:26:38 nmcl Exp $
 */

package com.arjuna.JavaArjunaLite.Implementation;

import com.arjuna.JavaGandiva.Common.ClassName;

public abstract class SemaphoreImple
{

    /* classic semaphore operations */

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
