/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Implementations.java,v 1.5 1998/07/06 13:26:36 nmcl Exp $
 */

package com.arjuna.JavaArjunaLite.Implementation;

import com.arjuna.JavaGandiva.Interface.Inventory;
import com.arjuna.JavaArjunaLite.Implementation.ObjectStore.ShadowingStoreSetup;
import com.arjuna.JavaArjunaLite.Implementation.ObjectStore.ActionStoreSetup;
import com.arjuna.JavaArjunaLite.Implementation.ObjectStore.ShadowNoFileLockStoreSetup;
import com.arjuna.JavaArjunaLite.Implementation.LockStore.BasicLockStoreSetup;
import com.arjuna.JavaArjunaLite.Implementation.LockStore.BasicPersistentLockStoreSetup;
import com.arjuna.JavaArjunaLite.Implementation.Semaphore.BasicSemaphoreSetup;

/*
 * Slightly different for historical reasons.
 */

import com.arjuna.JavaArjunaLite.Implementation.AbstractRecord.PersistenceRecordSetup;
import com.arjuna.JavaArjunaLite.Implementation.AbstractRecord.CadaverRecordSetup;

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
	    Inventory.inventory().addToList(new BasicLockStoreSetup());
	    Inventory.inventory().addToList(new BasicPersistentLockStoreSetup());
	    Inventory.inventory().addToList(new BasicSemaphoreSetup());	

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
	if (!_added)
	{
	    Inventory.inventory().addToList(new ShadowingStoreSetup());
	    Inventory.inventory().addToList(new ShadowNoFileLockStoreSetup());	
	    Inventory.inventory().addToList(new ActionStoreSetup());
	    Inventory.inventory().addToList(new BasicLockStoreSetup());
	    Inventory.inventory().addToList(new BasicPersistentLockStoreSetup());
	    Inventory.inventory().addToList(new BasicSemaphoreSetup());	

	    /*
	     * Now add various abstract records which crash recovery needs.
	     */

	    Inventory.inventory().addToList(new PersistenceRecordSetup());

	    _added = true;
	}
    }
    
}
