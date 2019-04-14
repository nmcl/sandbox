/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: Implementations.java,v 1.2 2003/06/26 10:03:45 nmcl Exp $
 */

package com.arjuna.ats.internal.arjuna;

import com.arjuna.ats.arjuna.gandiva.inventory.Inventory;

import com.arjuna.ats.internal.arjuna.objectstore.ShadowingStoreSetup;
import com.arjuna.ats.internal.arjuna.objectstore.ActionStoreSetup;
import com.arjuna.ats.internal.arjuna.objectstore.HashedActionStoreSetup;
import com.arjuna.ats.internal.arjuna.objectstore.ShadowNoFileLockStoreSetup;
import com.arjuna.ats.internal.arjuna.objectstore.HashedStoreSetup;
import com.arjuna.ats.internal.arjuna.objectstore.JDBCStoreSetup;
import com.arjuna.ats.internal.arjuna.objectstore.JDBCActionStoreSetup;
import com.arjuna.ats.internal.arjuna.objectstore.CacheStoreSetup;
import com.arjuna.ats.internal.arjuna.objectstore.CacheStoreSetup;

import com.arjuna.ats.internal.arjuna.gandiva.nameservice.JNSSetup;
import com.arjuna.ats.internal.arjuna.gandiva.nameservice.PNSSetup;

import com.arjuna.ats.internal.arjuna.PersistenceRecordSetup;
import com.arjuna.ats.internal.arjuna.CadaverRecordSetup;

/*
 * No naming or inventory implementations added yet.
 */

/**
 * Module specific class that is responsible for adding any implementations
 * to the inventory.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: Implementations.java,v 1.2 2003/06/26 10:03:45 nmcl Exp $
 * @since JTS 1.0.
 */

public class Implementations
{

public static synchronized boolean added ()
    {
	return _added;
    }
    
public static synchronized void initialise ()
    {
	if (!_added)
	{
	    Inventory.inventory().addToList(new ShadowingStoreSetup());
	    Inventory.inventory().addToList(new ShadowNoFileLockStoreSetup());
	    Inventory.inventory().addToList(new ActionStoreSetup());
	    Inventory.inventory().addToList(new HashedActionStoreSetup());
	    Inventory.inventory().addToList(new HashedStoreSetup());
	    Inventory.inventory().addToList(new JDBCStoreSetup());
	    Inventory.inventory().addToList(new JDBCActionStoreSetup());
	    Inventory.inventory().addToList(new CacheStoreSetup());

	    Inventory.inventory().addToList(new JNSSetup());
	    Inventory.inventory().addToList(new PNSSetup());

	    /*
	     * Now add various abstract records which crash recovery needs.
	     */

	    Inventory.inventory().addToList(new PersistenceRecordSetup());

	    _added = true;
	}
    }

private Implementations ()
    {
    }

private static boolean _added = false;

    static
    {
	initialise();
    }
    
}
