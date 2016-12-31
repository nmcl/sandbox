/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: BasicList.java,v 1.1 2000/02/25 14:02:54 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Template;

import com.arjuna.ArjunaCommon.Common.Uid;

public class BasicList extends SimpleList
{

public BasicList ()
    {
    }

public synchronized ListElement remove (Uid findMe)
    {
	SimpleListIterator iter = new SimpleListIterator(this);
	ListElement th = null;
    
	while ((th = iter.iterate()) != null)
	{
	    if (th.get_uid().equals(findMe))
	    {
		super.remove(th);
	    
		return th;
	    }
	}
    
	return null;
    }

public synchronized ListElement lookFor (Uid findMe)
    {
	SimpleListIterator iter = new SimpleListIterator(this);
	ListElement th = null;
    
	while ((th = iter.iterate()) != null)
	{
	    if (th.get_uid().equals(findMe))
	    {
		return th;
	    }
	}
    
	return null;
    }

public synchronized boolean lookFor (ListElement findMe)
    {
	return super.lookFor(findMe);
    }

public synchronized boolean remove (ListElement findMe)
    {
	return super.remove(findMe);
    }

};
