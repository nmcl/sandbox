/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OrderedListEntry.java,v 1.1.32.1 2000/12/21 11:20:56 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Template;

/*
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: OrderedListEntry.java,v 1.1.32.1 2000/12/21 11:20:56 nmcl Exp $
 * @since JTS 1.2.4.
 */

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
