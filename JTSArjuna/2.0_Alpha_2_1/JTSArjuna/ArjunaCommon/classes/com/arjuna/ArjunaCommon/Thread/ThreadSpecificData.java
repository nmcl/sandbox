/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ThreadSpecificData.java,v 1.1.6.1 2000/05/22 10:09:02 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Thread;

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
	_table.clear();
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
