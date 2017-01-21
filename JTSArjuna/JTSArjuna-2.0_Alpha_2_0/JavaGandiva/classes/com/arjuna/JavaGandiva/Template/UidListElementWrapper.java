/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: UidListElementWrapper.java,v 1.2 1998/07/06 13:31:11 nmcl Exp $
 */

package com.arjuna.JavaGandiva.Template;

import com.arjuna.JavaGandiva.Common.Uid;

public class UidListElementWrapper implements UidListElement
{

public UidListElementWrapper (Object o, Uid u)
    {
	_value = o;
	_uid = u;
    }

public UidListElementWrapper (Object o)
    {
	_value = o;
	_uid = new Uid();
    }
    
public Uid get_uid ()
    {
	return _uid;
    }

public Object value ()
    {
	return _value;
    }

private Object _value;
private Uid _uid;
    
};
