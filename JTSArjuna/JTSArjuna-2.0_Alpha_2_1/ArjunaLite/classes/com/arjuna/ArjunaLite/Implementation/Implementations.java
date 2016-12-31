/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: Implementations.java,v 1.1 2000/02/25 14:04:01 nmcl Exp $
 */

package com.arjuna.ArjunaLite.Implementation;

import com.arjuna.ArjunaCommon.Interface.Inventory;
import com.arjuna.ArjunaLite.Implementation.ObjectStore.ShadowingStoreSetup;
import com.arjuna.ArjunaLite.Implementation.ObjectStore.ActionStoreSetup;
import com.arjuna.ArjunaLite.Implementation.ObjectStore.ShadowNoFileLockStoreSetup;
import com.arjuna.ArjunaLite.Implementation.ObjectStore.HashedStoreSetup;
import com.arjuna.ArjunaLite.Implementation.LockStore.BasicLockStoreSetup;
import com.arjuna.ArjunaLite.Implementation.LockStore.BasicPersistentLockStoreSetup;
import com.arjuna.ArjunaLite.Implementation.Semaphore.BasicSemaphoreSetup;

/*
 * Slightly different for historical reasons.
 */

import com.arjuna.ArjunaLite.Implementation.AbstractRecord.PersistenceRecordSetup;
import com.arjuna.ArjunaLite.Implementation.AbstractRecord.CadaverRecordSetup;

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
	    Inventory.inventory().addToList(new HashedStoreSetup());
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
	    Inventory.inventory().addToList(new HashedStoreSetup());
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
