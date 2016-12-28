/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: SimpleListIterator.java,v 1.1 2000/02/25 14:03:02 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Template;

public class SimpleListIterator
{

public SimpleListIterator (SimpleList list)
    {
	nextEnt = list.head_;
    }

public synchronized ListElement iterate ()
    {
	ListElement th = null;

	if (nextEnt != null)
	{
	    th = nextEnt.theData;
	    nextEnt = nextEnt.next;
	}

	return th;
    }

private SimpleListEntry nextEnt;
    
};
