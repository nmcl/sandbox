/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ListElementWrapper.java,v 1.1 2003/06/19 10:50:39 nmcl Exp $
 */

package com.arjuna.ats.internal.arjuna.template;

import com.arjuna.ats.arjuna.common.Uid;

/**
 * For those objects which cannot provide get_uid, but which
 * need to be added to a list.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ListElementWrapper.java,v 1.1 2003/06/19 10:50:39 nmcl Exp $
 * @since JTS 1.0.
 */

public class ListElementWrapper implements ListElement
{

    /**
     * Create a new instance with the specified Uid.
     */

public ListElementWrapper (Object o, Uid u)
    {
	_value = o;
	_uid = u;
    }

    /**
     * Create a new instance. A new Uid will be automatically created
     * for this item.
     */

public ListElementWrapper (Object o)
    {
	_value = o;
 	_uid = new Uid();
    }

    /**
     * Return the Uid for the item.
     */

public Uid get_uid ()
    {
	return _uid;
    }

    /**
     * Return the item.
     */

public Object value ()
    {
	return _value;
    }

private Object _value;
private Uid    _uid;
    
}
