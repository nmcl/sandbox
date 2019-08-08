/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: SimpleList.java,v 1.1.28.1.4.1 2000/12/21 11:20:56 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Template;

/**
 * A SimpleList maintains entries in a linked list.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: SimpleList.java,v 1.1.28.1.4.1 2000/12/21 11:20:56 nmcl Exp $
 * @since JTS 1.0.
 */

public class SimpleList
{

    /**
     * Create a new, empty list.
     */

public SimpleList ()
    {
	head_ = null;
	entryCount = 0;
    }

public void finalize ()
    {
	empty();
    }

    /**
     * Empty the list.
     */

public synchronized void empty ()
    {
	ListElement te = null;

	while ((te = orderedPop()) != null)
	{
	}
    }

    /**
     * Return the head of the list.
     */

public synchronized ListElement head ()
    {
	if (head_ != null)
	    return head_.theData;
	else
	    return null;
    }

    /**
     * Returns the number of entries in the list.
     */

public synchronized int size ()
    {
	return entryCount;
    }

    /**
     * Is the specified entry in the list. Uses Object.equals
     * for comparison.
     */

public synchronized boolean lookFor (ListElement th)
    {
	if (th != null)
	{
	    SimpleListEntry te = head_;
        
	    while (te != null)
	    {
		/* exists already ? */
            
		if ((te.theData == th) ||
		    (te.equals(th)))
		{     
		    return true;
		}
                
		te = te.next;
	    }
	}
    
	return false;
    }

    /**
     * Push the specified item onto the list.
     */

public synchronized boolean push (ListElement th)
    {
	if (th != null)
	{
	    SimpleListEntry te = new SimpleListEntry(head_, th);
	
	    head_ = te;
	    entryCount++;
	    
	    return true;
	}
    
	return false;
    }

    /**
     * Add the specified item to the list. Same as push.
     * Will add the entry even if it is already in the list.
     */

public synchronized boolean add (ListElement th)
    {
	return push(th);
    }

    /**
     * Insert is like add except that it only adds once, i.e., if the entry
     * already exists it returns false. So, two different instances of
     * ListElement with the same value will not be allowed.
     */

public synchronized boolean insert (ListElement th)
    {
	if (th != null)
	{
	    SimpleListEntry te = head_;
	
	    while (te != null)
	    {
		/* exists already ? */

		if (th == te)
		{
		    return false;
		}
		
		te = te.next;
	    }
	
	    /* insert if not found */
	
	    te = new SimpleListEntry(head_, th);
	
	    head_ = te;
	    entryCount++;
	    
	    return true;
	}
    
	return false;
    }

    /**
     * Remove the specified entry. Returns an indication of
     * whether the item was in the list.
     */

public synchronized boolean remove (ListElement findMe)
    {
	SimpleListEntry te = head_;
	SimpleListEntry old = null;
	
	while (te != null)
	{
	    if (te.theData == findMe)
	    {
		if (old == null)
		    head_ = te.next;
		else
		    old.next = te.next;
	    
		te = null;
		entryCount--;
	    
		return true;
	    }
	
	    old = te;
	    te = te.next;
	}
    
	return false;
    }

    /**
     * Pop (remove) the first entry off the list.
     */

public synchronized ListElement orderedPop ()
    {
	ListElement th = null;
	
	if (head_ != null)
	{
	    SimpleListEntry te = head_;
	    
	    head_ = te.next;
	    th = te.theData;
	    entryCount--;

	    te = null;
	}
    
	return th;
    }

protected SimpleListEntry head_;
protected int             entryCount;

};
