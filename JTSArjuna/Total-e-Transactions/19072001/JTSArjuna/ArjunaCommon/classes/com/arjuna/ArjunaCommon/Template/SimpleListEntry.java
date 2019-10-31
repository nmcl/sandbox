/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: SimpleListEntry.java,v 1.2.32.1 2000/12/21 11:20:57 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Template;

/*
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: SimpleListEntry.java,v 1.2.32.1 2000/12/21 11:20:57 nmcl Exp $
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
