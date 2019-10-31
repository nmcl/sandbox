/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OrderedListIterator.java,v 1.1.28.1.4.1 2000/12/21 11:20:56 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Template;

/**
 * An iterator for OrderedLists.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: OrderedListIterator.java,v 1.1.28.1.4.1 2000/12/21 11:20:56 nmcl Exp $
 * @since JTS 1.2.4.
 */

public class OrderedListIterator
{

    /**
     * Create a new iterator which will iterate over the specified list.
     */

public OrderedListIterator (OrderedList L)
    {
	ptr = L._headOfList;
    }

public void finalize ()
    {
	ptr = null;
    }

    /**
     * Return the next item in the list.
     */

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
 
}
