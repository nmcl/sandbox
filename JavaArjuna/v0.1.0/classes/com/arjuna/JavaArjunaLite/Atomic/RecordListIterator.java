/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RecordListIterator.java,v 1.3 1998/07/06 13:26:30 nmcl Exp $
 */

package com.arjuna.JavaArjunaLite.Atomic;

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
