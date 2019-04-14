/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: BasicList.java,v 1.1 2003/06/19 10:50:38 nmcl Exp $
 */

package com.arjuna.ats.internal.arjuna.template;

import com.arjuna.ats.arjuna.common.Uid;

/**
 * A BasicList extends the concept of a SimpleList by
 * searching and storing entries based upon their Uids.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: BasicList.java,v 1.1 2003/06/19 10:50:38 nmcl Exp $
 * @since JTS 1.0.
 */

public class BasicList extends SimpleList
{

    /**
     * Create a new BasicList.
     */

public BasicList ()
    {
    }

    /**
     * Remove the entry (and return it) which has the specified
     * Uid.
     */

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

    /**
     * Return the entry which has the specified Uid.
     */

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

}

