/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: SimpleListEntry.java,v 1.1 2003/06/19 10:50:39 nmcl Exp $
 */

package com.arjuna.ats.internal.arjuna.template;

/*
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: SimpleListEntry.java,v 1.1 2003/06/19 10:50:39 nmcl Exp $
 * @since JTS 1.0.
 */

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
protected ListElement     theData;

};
