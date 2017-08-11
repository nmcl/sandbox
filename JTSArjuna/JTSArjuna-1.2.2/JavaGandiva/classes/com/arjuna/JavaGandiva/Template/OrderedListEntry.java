/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OrderedListEntry.java,v 1.2 1998/07/06 13:31:08 nmcl Exp $
 */

package com.arjuna.JavaGandiva.Template;

class OrderedListEntry
{
    
public OrderedListEntry (OrderedListElement e, OrderedListEntry n)
    {
	_theElement = e;
	_next = n;
    }
    
public final OrderedListElement car ()
    {
	return _theElement;
    }

public final OrderedListEntry cdr ()
    {
	return _next;
    }

public final void setfCdr (OrderedListEntry n)
    {
	_next = n;
    }

private OrderedListElement _theElement;
private OrderedListEntry _next;

};
