/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: SimpleUidList.java,v 1.4 1998/07/06 13:31:09 nmcl Exp $
 */

package com.arjuna.JavaGandiva.Template;

public class SimpleUidList
{

public SimpleUidList ()
    {
	head_ = null;
	entryCount = 0;
    }

public void finalize ()
    {
	empty();
    }

public synchronized void empty ()
    {
	UidListElement te = null;

	while ((te = orderedPop()) != null)
	{
	}
    }

public synchronized UidListElement head ()
    {
	if (head_ != null)
	    return head_.theData;
	else
	    return null;
    }

public synchronized int size ()
    {
	return entryCount;
    }

public synchronized boolean lookFor (UidListElement th)
    {
	if (th != null)
	{
	    SimpleListEntry te = head_;
        
	    while (te != null)
	    {
		/* exists already ? */
            
		if (te.theData == th)
		{     
		    return true;
		}
                
		te = te.next;
	    }
	}
    
	return false;
    }

public synchronized boolean push (UidListElement th)
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

public synchronized boolean add (UidListElement th)
    {
	return push(th);
    }

    /*
     * Insert is like add except that it only adds once. i.e., if the entry
     * already exists it returns false;
     */

public synchronized boolean insert (UidListElement th)
    {
	if (th != null)
	{
	    SimpleListEntry te = head_;
	
	    while (te != null)
	    {
		/* exists already ? */
	    
		if (te.theData == th)
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

public synchronized boolean remove (UidListElement findMe)
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

public synchronized UidListElement orderedPop ()
    {
	UidListElement th = null;
	
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
protected int entryCount;

};

class SimpleListEntry
{

public SimpleListEntry (SimpleListEntry old, UidListElement th)
    {
	next = old;
	theData = th;
    }

protected SimpleListEntry next;
protected UidListElement theData;

};
