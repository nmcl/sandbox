/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: SimpleUidListIterator.java,v 1.4 1998/07/06 13:31:10 nmcl Exp $
 */

package com.arjuna.JavaArjuna.Template;

public class SimpleUidListIterator
{

public SimpleUidListIterator (SimpleUidList list)
    {
	nextEnt = list.head_;
    }

public synchronized UidListElement iterate ()
    {
	UidListElement th = null;

	if (nextEnt != null)
	{
	    th = nextEnt.theData;
	    nextEnt = nextEnt.next;
	}

	return th;
    }

private SimpleListEntry nextEnt;
    
};
