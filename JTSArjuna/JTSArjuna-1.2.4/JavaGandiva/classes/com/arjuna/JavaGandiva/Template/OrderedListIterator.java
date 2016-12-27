/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OrderedListIterator.java,v 1.2 1998/07/06 13:31:09 nmcl Exp $
 */

package com.arjuna.JavaGandiva.Template;

public class OrderedListIterator
{

public OrderedListIterator (OrderedList L)
    {
	ptr = L._headOfList;
    }

public void finalize ()
    {
	ptr = null;
    }

public synchronized OrderedListElement iterate ()
    {
	if (ptr != null)
	{
	    OrderedListEntry p = ptr;
	    ptr = ptr.cdr();
	    return p.car();
	}

	return null;
    }

private OrderedListEntry ptr;
 
};
