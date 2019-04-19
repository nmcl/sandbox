/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: LockList.java,v 1.1 2003/06/19 11:56:10 nmcl Exp $
 */

package com.arjuna.ats.txoj;

public class LockList
{

public LockList ()
    {
	count = 0;
	head = null;
    }

public void finalize ()
    {
	Lock temp = null;

	while ((temp = pop()) != null)
	    temp = null;  // temp.finalize() ?
    }

    /*
     * Insert a new Lock. This returns TRUE if the insertion occurred, false
     * otherwise. Insertion fails if a matching lock already exists in the
     * list.
     */

public final boolean insert (Lock newlock)
    {
	LockListIterator next = new LockListIterator(this);
	Lock current = null;

	while ((current = next.iterate()) != null)
	{
	    if (current.equals(newlock))
	    {
		return false;
	    }
	}

	push(newlock);
	
	return true;
    }

    /*
     * Pop the first element off the list and return it.
     */

public final Lock pop ()
    {
	Lock current;

	if (count == 0)
	    return null;

	current = (Lock) head;
	count--;
	head = head.getLink();
	current.setLink(null);
	
	return current;
    }

    /*
     *  Push a new element at the head of the list. First set the link
     *  field to be the old head, and then set head to be the new element.
     */

public final void push (Lock newLock)
    {
	newLock.setLink(head);
	head = newLock;
	count++;
    }

    /*
     * Discard the element following the one pointed at. If it is the
     * first element (current = 0) then simply change the head pointer.
     * Beware if current points at the last element or the list is empty!
     * This probably indicates a bug in the caller.
     */

public final void forgetNext (Lock current)
    {
	if (count > 0)			/* something there to forget */
	{
	    if (current == null)
		head = head.getLink();
	    else
	    {
		Lock nextOne = current.getLink();
	    
		/* See if at list end */

		if (nextOne != null)
		    current.setLink(nextOne.getLink());
		else
		{
		    /*
		     * Probably an error - being asked to forget element
		     * after end of list
		     */
		    count++;
		    current.setLink(null);	/* force end of list */
		}
	    }
	    
	    count--;
	}
    }

public final int entryCount ()
    {
	return count;
    }

protected Lock head;
    
private int count;
    
}
