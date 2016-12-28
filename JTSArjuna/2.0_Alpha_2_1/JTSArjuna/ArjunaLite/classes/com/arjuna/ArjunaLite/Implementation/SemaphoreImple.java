/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: SemaphoreImple.java,v 1.1 2000/02/25 14:04:03 nmcl Exp $
 */

package com.arjuna.ArjunaLite.Implementation;

import com.arjuna.ArjunaCommon.Common.ClassName;

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
