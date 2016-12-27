/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BasicListIterator.java,v 1.2 1998/07/06 13:31:03 nmcl Exp $
 */

package com.arjuna.JavaGandiva.Template;

public class BasicListIterator
{

public BasicListIterator (BasicList l)
    {
	_list = l;
	_current = _list._head;
    }

public synchronized Object next ()
    {
	if (_current != null)
	{
	    Object ptr = _current.value();

	    _current = _current.next();

	    return ptr;
	}
	else
	    return null;
    }

private BasicList _list;
private ObjectElement _current;
    
};
