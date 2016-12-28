/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  

 * $Id: SimpleList.java,v 1.1 2000/02/25 14:03:01 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Template;

public class SimpleList
{

public SimpleList ()
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
	ListElement te = null;

	while ((te = orderedPop()) != null)
	{
	}
    }

public synchronized ListElement head ()
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

public synchronized boolean lookFor (ListElement th)
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

public synchronized boolean add (ListElement th)
    {
	return push(th);
    }

    /*
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
protected int entryCount;

};
