/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: Implementations.java,v 1.1.2.1.2.1 2000/11/21 12:21:38 nmcl Exp $
 */

package com.arjuna.JDBC2.Implementation;

import com.arjuna.ArjunaCommon.Interface.Inventory;

public class Implementations
{

public Implementations ()
    {
	Implementations.initialise();
    }
    
public static synchronized boolean added ()
    {
	return _added;
    }
    
public static synchronized void initialise ()
    {
	if (!_added)
	{
	    /*
	     * Now add various abstract records which crash recovery needs.
	     */

	    Inventory.inventory().addToList(new com.arjuna.JDBC2.Implementation.ObjectStore.JDBCStoreSetup());
	    Inventory.inventory().addToList(new com.arjuna.JDBC2.Implementation.ObjectStore.JDBCActionStoreSetup());

	    _added = true;
	}
    }

private static boolean _added = false;

    static
    {
	Implementations.initialise();
    }
    
}
