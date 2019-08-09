/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: Implementations.java,v 1.1.4.1.2.1.2.1.2.1 2000/12/21 11:21:08 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Implementation;

import com.arjuna.ArjunaCommon.Interface.Inventory;
import com.arjuna.ArjunaCore.Implementation.ObjectStore.ShadowingStoreSetup;
import com.arjuna.ArjunaCore.Implementation.ObjectStore.ActionStoreSetup;
import com.arjuna.ArjunaCore.Implementation.ObjectStore.HashedActionStoreSetup;
import com.arjuna.ArjunaCore.Implementation.ObjectStore.ShadowNoFileLockStoreSetup;
import com.arjuna.ArjunaCore.Implementation.ObjectStore.HashedStoreSetup;
import com.arjuna.ArjunaCore.Implementation.LockStore.BasicLockStoreSetup;
import com.arjuna.ArjunaCore.Implementation.LockStore.BasicPersistentLockStoreSetup;
import com.arjuna.ArjunaCore.Implementation.Semaphore.BasicSemaphoreSetup;

/*
 * Slightly different for historical reasons.
 */

import com.arjuna.ArjunaCore.Implementation.AbstractRecord.PersistenceRecordSetup;
import com.arjuna.ArjunaCore.Implementation.AbstractRecord.CadaverRecordSetup;

/**
 * Module specific class that is responsible for adding any implementations
 * to the inventory.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: Implementations.java,v 1.1.4.1.2.1.2.1.2.1 2000/12/21 11:21:08 nmcl Exp $
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
	initialise();
    }
    
}
