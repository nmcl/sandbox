/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: SimpleListEntry.java,v 1.2 2000/03/07 09:43:21 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Template;

class SimpleListEntry
{

public SimpleListEntry (SimpleListEntry old, ListElement th)
    {
	next = old;
	theData = th;
    }

public void finalize ()
    {
	theData = null;
    }
    
protected SimpleListEntry next;
protected ListElement theData;

};
