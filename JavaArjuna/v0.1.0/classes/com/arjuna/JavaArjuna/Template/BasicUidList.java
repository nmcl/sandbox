/*
 * Copyright (C) 1995, 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BasicUidList.java,v 1.4 1998/07/06 13:31:04 nmcl Exp $
 */

package com.arjuna.JavaArjuna.Template;

import com.arjuna.JavaArjuna.Common.Uid;

public class BasicUidList extends SimpleUidList
{

public BasicUidList ()
    {
    }

public synchronized UidListElement remove (Uid findMe)
    {
	SimpleUidListIterator iter = new SimpleUidListIterator(this);
	UidListElement th = null;
    
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

public synchronized UidListElement lookFor (Uid findMe)
    {
	SimpleUidListIterator iter = new SimpleUidListIterator(this);
	UidListElement th = null;
    
	while ((th = iter.iterate()) != null)
	{
	    if (th.get_uid().equals(findMe))
	    {
		return th;
	    }
	}
    
	return null;
    }

public synchronized boolean lookFor (UidListElement findMe)
    {
	return super.lookFor(findMe);
    }

public synchronized boolean remove (UidListElement findMe)
    {
	return super.remove(findMe);
    }

};
