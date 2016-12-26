/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ThreadSpecificData.java,v 1.1.6.1.24.1.4.1 2000/12/21 11:20:57 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Thread;

import java.lang.Thread;
import java.util.Hashtable;

/**
 * A thread specific data implementation. An instance of this
 * class will maintain data on behalf of each thread. One thread
 * cannot gain access to another thread's data.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ThreadSpecificData.java,v 1.1.6.1.24.1.4.1 2000/12/21 11:20:57 nmcl Exp $
 * @since JTS 1.0.
 */

public class ThreadSpecificData
{

    /**
     * Create a new instance. Space will be allocated dynamically
     * as required.
     */

public ThreadSpecificData ()
    {
	_table = new Hashtable();
    }

    /**
     * Create a new instance with the specified size. Space will be
     * increased dynamically as required.
     */

public ThreadSpecificData (int v)
    {
	_table = new Hashtable(v);
    }

public void finalize ()
    {
	_table.clear();
	_table = null;
    }

    /**
     * Associate the specified data with the current thread.
     */

public synchronized void setSpecific (Object o)
    {
	_table.put(Thread.currentThread(), o);
    }
    
    /**
     * Return the data associated with the current thread. If no
     * association has occurred then null will be returned. Obviously
     * if null was legally associated with the thread then it is not
     * possible to determine whether data was stored.
     */

public synchronized Object getSpecific ()
    {
	return _table.get(Thread.currentThread());
    }

    /**
     * Returns whether data has been associated with the current
     * thread.
     */

public synchronized boolean hasSpecific ()
    {
	return _table.containsKey(Thread.currentThread());
    }

    /**
     * Remove the data associated with the current thread.
     */

public synchronized void removeSpecific ()
    {
	_table.remove(Thread.currentThread());
    }

private Hashtable _table;
 
}
