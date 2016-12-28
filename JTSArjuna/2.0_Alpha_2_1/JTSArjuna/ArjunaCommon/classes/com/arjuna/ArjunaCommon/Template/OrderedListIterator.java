/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OrderedListIterator.java,v 1.1 2000/02/25 14:03:00 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Template;

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
