/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: SimpleListIterator.java,v 1.1 2003/06/19 10:50:39 nmcl Exp $
 */

package com.arjuna.ats.internal.arjuna.template;

/**
 * An iterator for SimpleLists.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: SimpleListIterator.java,v 1.1 2003/06/19 10:50:39 nmcl Exp $
 * @since JTS 1.0.
 */

public class SimpleListIterator
{

    /**
     * Create a new iterator, which whill be used to iterate
     * over the provided list.
     */

public SimpleListIterator (SimpleList list)
    {
	nextEnt = list.head_;
    }

    /**
     * Return the next entry in the list.
     */

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
