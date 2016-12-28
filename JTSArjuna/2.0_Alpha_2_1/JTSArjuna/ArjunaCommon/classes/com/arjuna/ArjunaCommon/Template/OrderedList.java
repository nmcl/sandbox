/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OrderedList.java,v 1.1 2000/02/25 14:02:58 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Template;

public class OrderedList
{

public OrderedList ()
    {
	_headOfList = null;
	_entryCount = 0;
	_increasing = true;
    }
    
public OrderedList (boolean increasing)
    {
	_headOfList = null;
	_entryCount = 0;
	_increasing = increasing;
    }

public void finalize ()
    {
	empty();
    }

public final synchronized boolean insert (OrderedListElement p)
    {
	return insert(p, false);
    }
    
public final synchronized boolean insert (OrderedListElement p, boolean prior)
    {
	if (p == null)  // error condition first
	    return false;
    
	// If list is empty, insert at head

	if (_entryCount == 0)
	{
	    _headOfList = new OrderedListEntry(p, _headOfList);
	    _entryCount++;
	    
	    return true;
	}

	// Try to insert before (if there is anything scheduled later)

	OrderedListIterator iter = new OrderedListIterator(this);
	OrderedListElement prev = null;
    
	for (OrderedListElement q = iter.iterate(); (q != null); prev = q, q = iter.iterate())
	{
	    if (_increasing)
	    {
		if (prior)
		{
		    if (q.equals(p) || (q.greaterThan(p)))
		    {
			return insertBefore(p, q);
		    }
		}
		else
		{
		    if (q.greaterThan(p))
			return insertBefore(p, q);
		}
	    }
	    else
	    {
		if (prior)
		{
		    if (q.equals(p) || (q.lessThan(p)))
		    {
			return insertBefore(p, q);
		    }
		}
		else
		{
		    if (q.lessThan(p))
			return insertBefore(p, q);
		}
	    }
	}

	// Got to insert at the end (currently pointed at by 'prev')

	return insertAfter(p, prev);
    }
    
public final synchronized boolean insertBefore (OrderedListElement toInsert, OrderedListElement before)
    {
	for (OrderedListEntry prev = null, p = _headOfList; (p != null); prev = p, p = p.cdr())
	{
	    if (p.car() == before)  // we mean '==' rather than equals
	    {
		OrderedListEntry newcons = new OrderedListEntry(toInsert, p);
		
		if (prev != null)
		    prev.setfCdr(newcons);
		else
		    _headOfList = newcons;

		_entryCount++;

		return true;
	    }
	}
    
	return false;
    }

public final synchronized boolean insertAfter (OrderedListElement toInsert, OrderedListElement after)
    {
        for (OrderedListEntry p = _headOfList; (p != null); p = p.cdr())
	{
	    if (p.car() == after)  // we mean '==' rather than equals
	    {
		OrderedListEntry newcons = new OrderedListEntry(toInsert, p.cdr());
		
		p.setfCdr(newcons);

		_entryCount++;
		
		return true;
	    }
	}
    
	return false;
    }

public final synchronized boolean remove (OrderedListElement element)
    {
	// Take care of boundary condition - empty list
	if ((_headOfList == null) || (element == null))
	    return false;

	for (OrderedListEntry prev = null, ptr = _headOfList; (ptr != null); prev = ptr, ptr = ptr.cdr())
	{
	    if (ptr.car() == element)  // we mean '==' rather than equals
	    {
		OrderedListEntry oldcons = ptr;
	    
		// unlink the cons cell for the element we're removing
		
		if (prev != null)
		    prev.setfCdr(ptr.cdr());
		else
		    _headOfList = ptr.cdr();

		_entryCount--;
	    
		// flush the dead cons cell
		oldcons = null;
	    
		return true;
	    }
	}

	return false;
    }

public final synchronized OrderedListElement orderedPop ()
    {
	if (_headOfList != null)
	{
	    OrderedListEntry remove = _headOfList;
	    OrderedListElement p = _headOfList.car();

	    _headOfList = remove.cdr();
	    _entryCount--;

	    remove = null;
	    
	    return p;
	}
	else
	    return null;
    }
    
public final synchronized void empty ()
    {
	OrderedListElement te;

	while ((te = orderedPop()) != null)
	{
	    te = null;
	}
    }

public final synchronized long size ()
    {
	return _entryCount;
    }

protected OrderedListEntry _headOfList;
protected long _entryCount;
    
private boolean _increasing;

};


