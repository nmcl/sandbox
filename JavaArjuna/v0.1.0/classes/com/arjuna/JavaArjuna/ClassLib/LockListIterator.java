/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LockListIterator.java,v 1.3 1998/07/06 13:26:21 nmcl Exp $
 */

package com.arjuna.JavaArjuna.ClassLib;

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

    /*
     * https://github.com/nmcl/sandbox/issues/74
public void finalize ()
    {
	next = null;
    }
    */

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
