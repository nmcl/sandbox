/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: BasicPersistentLockStore.java,v 1.1 2000/02/25 14:04:05 nmcl Exp $
 */

package com.arjuna.ArjunaLite.Implementation.LockStore;

import com.arjuna.ArjunaLite.ArjunaLiteNames;
import com.arjuna.ArjunaLite.Implementation.LockStoreImple;
import com.arjuna.ArjunaLite.Interface.ObjectStore;
import com.arjuna.ArjunaLite.Common.ArjunaLiteEnvironment;
import com.arjuna.ArjunaLite.Common.Configuration;
import com.arjuna.ArjunaCommon.Common.*;

import com.arjuna.ArjunaLite.Common.ObjectStoreException;
import com.arjuna.ArjunaLite.Common.LockStoreException;

public class BasicPersistentLockStore extends LockStoreImple
{

    /*
     * Ignore key as we can make use of the basic type information for
     * this type of store. Really only need it for shared memory.
     */
    
public BasicPersistentLockStore (String key)
    {
	if (lockStoreLocation == null)
	    lockStoreLocation = PropertyManager.getProperty(ArjunaLiteEnvironment.LOCKSTORE_DIR,
							    com.arjuna.ArjunaLite.Common.Configuration.lockStoreRoot());
	
	_key = lockStoreLocation;

	/*
	 * Use the ShadowingStore since it has file-level locking which
	 * we require. The default object store assumes locking is provided
	 * entirely by the object.
	 */
	
	_lockStore = new ObjectStore(ArjunaLiteNames.Implementation_ObjectStore_ShadowingStore(), _key);
    }

public InputObjectState read_state (Uid u, String tName) throws LockStoreException
    {
	try
	{
	    return _lockStore.read_committed(u, tName);
	}
	catch (ObjectStoreException e)
	{
	    throw new LockStoreException("Persistent store error.");
	}
    }
    
public boolean remove_state (Uid u, String tName)
    {
	try
	{
	    return _lockStore.remove_committed(u, tName);
	}
	catch (ObjectStoreException e)
	{
	    return false;
	}
    }
    
public boolean write_committed (Uid u, String tName, OutputObjectState state)
    {
	try
	{
	    return _lockStore.write_committed(u, tName, state);
	}
	catch (ObjectStoreException e)
	{
	    return false;
	}
    }

public ClassName className ()
    {
	return ArjunaLiteNames.Implementation_LockStore_BasicPersistentLockStore();
    }

public static ClassName name ()
    {
	return ArjunaLiteNames.Implementation_LockStore_BasicPersistentLockStore();
    }    

public static final BasicPersistentLockStore create (Object[] param)
    {
	if (param == null)
	    return null;
	
	String key = (String) param[0];

	return new BasicPersistentLockStore(key);
    }
    
private String _key;
private ObjectStore _lockStore;

private static String lockStoreLocation = null;
    
};
