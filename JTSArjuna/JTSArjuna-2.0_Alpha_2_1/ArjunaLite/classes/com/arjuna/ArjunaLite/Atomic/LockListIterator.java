/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: LockListIterator.java,v 1.1 2000/02/25 14:03:49 nmcl Exp $
 */

package com.arjuna.ArjunaLite.Atomic;

/*
 * Default visibility.
 */

class LockListIterator
{

public LockListIterator (LockList L)
    {
	currentList = L;
	next = L.head;
    }
    
public void finalize ()
    {
	next = null;
    }

public final synchronized Lock iterate ()
    {
	Lock current = next;
	
	if (current == null)
	{
	    return null;
	}
	else
	    next = current.getLink();

	return current;
    }

public final synchronized void reset ()
    {
	next = null;
    }
    
private LockList currentList;
private Lock next;

}
