/*
 * Copyright (C) 1999,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: SimpleListEntry.java,v 1.1.2.1 1999/06/16 08:20:27 nmcl Exp $
 */

package com.arjuna.JavaGandiva.Template;

class SimpleListEntry
{

public SimpleListEntry (SimpleListEntry old, UidListElement th)
    {
	next = old;
	theData = th;
    }

protected SimpleListEntry next;
protected UidListElement theData;

};
