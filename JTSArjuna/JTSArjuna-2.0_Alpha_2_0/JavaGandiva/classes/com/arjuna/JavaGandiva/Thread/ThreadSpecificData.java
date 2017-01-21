/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ThreadSpecificData.java,v 1.3 1998/07/06 13:31:12 nmcl Exp $
 */

package com.arjuna.JavaGandiva.Thread;

import java.lang.Thread;
import java.util.Hashtable;

public class ThreadSpecificData
{

public ThreadSpecificData ()
    {
	_table = new Hashtable();
    }

public ThreadSpecificData (int v)
    {
	_table = new Hashtable(v);
    }

public void finalize ()
    {
	_table = null;
    }

public synchronized void setSpecific (Object o)
    {
	_table.put(Thread.currentThread(), o);
    }

public synchronized Object getSpecific ()
    {
	return _table.get(Thread.currentThread());
    }

public synchronized void removeSpecific ()
    {
	_table.put(Thread.currentThread(), null);
    }

private Hashtable _table;

};
