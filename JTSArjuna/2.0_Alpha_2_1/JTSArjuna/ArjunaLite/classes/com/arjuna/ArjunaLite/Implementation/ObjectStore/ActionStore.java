/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ActionStore.java,v 1.1 2000/02/25 14:04:07 nmcl Exp $
 */

package com.arjuna.ArjunaLite.Implementation.ObjectStore;

import com.arjuna.ArjunaLite.ArjunaLiteNames;
import com.arjuna.ArjunaLite.Common.ArjunaLiteEnvironment;
import com.arjuna.ArjunaLite.Atomic.*;
import com.arjuna.ArjunaLite.Interface.ObjectStore;
import com.arjuna.ArjunaCommon.Common.*;
import java.io.File;

import com.arjuna.ArjunaLite.Common.ObjectStoreException;
import java.io.IOException;

/*
 * Overrides the default lock/unlock implementations to do nothing.
 * This is ok since only a single user (the coordinator) can ever be
 * manipulating the action's state.
 */

public class ActionStore extends ShadowNoFileLockStore
{

public int typeIs ()
    {
	return ObjectStoreType.ACTION;
    }
    
    /*
     * Determine current state of object. Assumes that genPathName allocates
     * enough extra space to allow extra chars to be added.
     * Action stores only store committed objects
     */

public int currentState (Uid objUid, String tName) throws ObjectStoreException
    {
	int theState = ObjectStore.OS_UNKNOWN;
    
	if (storeValid())
	{
	    String path = genPathName(objUid, tName, ObjectStore.OS_ORIGINAL);

	    if (exists(path))
		theState = ObjectStore.OS_COMMITTED;

	    path = null;
	}

	return theState;
    }

    /*
     * The following operation commits a previous write_state operation which
     * was made with the SHADOW StateType argument. This is achieved by
     * renaming the shadow and removing the hidden version.
     */

public synchronized boolean commit_state (Uid objUid,
					  String tName) throws ObjectStoreException
    {
	boolean result = false;

	/* Bail out if the object store is not set up */

	if (!storeValid())
	    return false;

	if (currentState(objUid, tName) == ObjectStore.OS_COMMITTED)
	    result = true;
    
	return result;
    }

public boolean hide_state (Uid u, String tn) throws ObjectStoreException
    {
	return false;
    }

public boolean reveal_state (Uid u, String tn) throws ObjectStoreException
    {
	return false;
    }

public InputObjectState read_committed (Uid storeUid, String tName) throws ObjectStoreException
    {
	return super.read_committed(storeUid, tName);
    }

public InputObjectState read_uncommitted (Uid u, String tn) throws ObjectStoreException
    {
	return null;
    }

public boolean remove_committed (Uid storeUid, String tName) throws ObjectStoreException
    {
	return super.remove_committed(storeUid, tName);
    }

public boolean remove_uncommitted (Uid u, String tn) throws ObjectStoreException
    {
	return false;
    }

public boolean write_committed (Uid storeUid, String tName, OutputObjectState state) throws ObjectStoreException
    {
	return super.write_committed(storeUid, tName, state);
    }

public boolean write_uncommitted (Uid u, String tn, OutputObjectState s) throws ObjectStoreException
    {
	return false;
    }

public ClassName className ()
    {
	return ArjunaLiteNames.Implementation_ObjectStore_ActionStore();
    }

public static ClassName name ()
    {
	return ArjunaLiteNames.Implementation_ObjectStore_ActionStore();	
    }    

    /*
     * Have to return as a ShadowingStore because of
     * inheritence.
     */
    
public static ShadowingStore create ()
    {
	return new ActionStore("");
    }

public static ShadowingStore create (Object[] param)
    {
	if (param == null)
	    return null;

	String location = (String) param[0];

	return new ActionStore(location);
    }
    
protected ActionStore (String locationOfStore)
    {
	super();

	try
	{
	    setupStore(locationOfStore);
	}
	catch (ObjectStoreException e)
	{
	    System.out.println(e.getMessage());
	    System.exit(0);
	}
    }
    
protected ActionStore ()
    {
	super();
    }

protected synchronized boolean setupStore (String location) throws ObjectStoreException
    {
	if (!checkSync)
	{
	    String syncOpt = PropertyManager.getProperty(ArjunaLiteEnvironment.TRANSACTION_SYNC);

	    if (syncOpt != null)
	    {
		if (syncOpt.compareTo("OFF") == 0)
		    syncOff();
		else
		    syncOn();
	    }
	    else
		syncOn();

	    checkSync = true;
	}

	return super.setupStore(location);
    }
    
private static boolean checkSync = false;
    
}
