/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: RecordListIterator.java,v 1.1 2000/02/25 14:03:54 nmcl Exp $
 */

package com.arjuna.ArjunaLite.Atomic;

class RecordListIterator
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

public final synchronized AbstractRecord iterate ()
    {
	if (curElem == null)
	    curElem = curList.listHead;
	else
	    curElem = curElem.getNext();
	
	return curElem;
    }

private AbstractRecord curElem;
private RecordList curList;

}
