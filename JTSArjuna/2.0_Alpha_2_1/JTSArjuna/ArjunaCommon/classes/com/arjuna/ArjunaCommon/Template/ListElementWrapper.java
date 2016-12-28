/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ListElementWrapper.java,v 1.1 2000/02/25 14:02:58 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Template;

import com.arjuna.ArjunaCommon.Common.Uid;

/*
 * For those objects which cannot provide get_uid.
 */

public class ListElementWrapper implements ListElement
{

public ListElementWrapper (Object o, Uid u)
    {
	_value = o;
	_uid = u;
    }

public ListElementWrapper (Object o)
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
