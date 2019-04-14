/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OrderedList.java,v 1.1 2003/06/19 10:50:39 nmcl Exp $
 */

package com.arjuna.ats.internal.arjuna.template;

/**
 * An ordered list stores items in an order defined by the objects
 * themselves. When an item is added, the implementation scans the list
 * and asks all currently registered items whether they are "equal",
 * "less than", or "greater than" the new item, and inserts at a position
 * determined by these answers (or at the end of the list).
 *
 * A list can be ordered in an increasing manner, or decreasing.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: OrderedList.java,v 1.1 2003/06/19 10:50:39 nmcl Exp $
 * @since JTS 1.2.4.
 */

public class OrderedList
{

    /**
     * Create a new increasing ordered list.
     */

public OrderedList ()
    {
	this(true);
    }
    
    /**
     * Create a new list. If the parameter is true, then it will be
     * ordered increasing, otherwise decreasing.
     */

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

    /**
     * Insert the item in the list. If items are found to be equal then
     * this one will be inserted at the end of that portion of the list.
     */

public final synchronized boolean insert (OrderedListElement p)
    {
	return insert(p, false);
    }

    /**
     * Insert the item in the list. If items are found to be equal then
     * this one will be inserted at the start of that portion of the list if
     * the boolean parameter is true, and at the end otherwise.
     */

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

    /**
     * Insert the item before the specified item. If the specified item is
     * not in the list then insertion will fail and false will be
     * returned.
     */

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

    /**
     * Insert the item after the specified item. If the specified item is
     * not in the list then insertion will fail and false will be
     * returned.
     */

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

    /**
     * Remove the specified item from the list. Return an indication
     * of whether or not the item was in the list.
     */

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

    /**
     * Remove and return the first item in the list.
     */

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

    /**
     * Empty the list.
     */

public final synchronized void empty ()
    {
	OrderedListElement te;

	while ((te = orderedPop()) != null)
	{
	    te = null;
	}
    }

    /**
     * Return the number of items in the list.
     */

public final synchronized long size ()
    {
	return _entryCount;
    }

protected OrderedListEntry _headOfList;
protected long             _entryCount;
    
private boolean _increasing;

}

