/*
 * Copyright (C) 1996, 1997, 1998, 1999,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ObjectElement.java,v 1.1.2.1 1999/05/21 14:46:22 nmcl Exp $
 */

package com.arjuna.JavaArjuna.Template;

class ObjectElement
{
    
public ObjectElement (Object t)
    {
	_object = t;
	_next = null;
    }

public Object value ()
    {
	return _object;
    }
    
public ObjectElement next ()
    {
	return _next;
    }

public void next (ObjectElement n)
    {
	_next = n;
    }
    
private Object _object;
private ObjectElement _next;
    
};
