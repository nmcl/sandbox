/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: RecordListIterator.java,v 1.1.4.1.6.2 2000/12/21 11:21:04 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Atomic;

/**
 * A basic iterator for RecordList instances.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: RecordListIterator.java,v 1.1.4.1.6.2 2000/12/21 11:21:04 nmcl Exp $
 * @since JTS 1.0.
 */

public class RecordListIterator
{

public RecordListIterator (RecordList R)
    {
	curElem = null;
	curList = R;
    }
    
public final synchronized void restart ()
    {
	curElem = null;
    }

    /**
     * @return the next entry in the list.
     */

public final synchronized AbstractRecord iterate ()
    {
	if (curElem == null)
	    curElem = curList.listHead;
	else
	    curElem = curElem.getNext();
	
	return curElem;
    }

private AbstractRecord curElem;
private RecordList     curList;

}
