/*
 * Copyright (C) 1996, 1997, 1998, 1999,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BasicPersistentLockStore.javatmpl,v 1.1.8.1 1999/08/25 10:49:19 nmcl Exp $
 */

package com.arjuna.JavaArjunaLite.Implementation.LockStore;

import com.arjuna.JavaArjunaLite.JavaArjunaLiteNames;
import com.arjuna.JavaArjunaLite.Implementation.LockStoreImple;
import com.arjuna.JavaArjunaLite.Interface.ObjectStore;
import com.arjuna.JavaGandiva.Common.*;

import com.arjuna.JavaArjunaLite.Common.ObjectStoreException;
import com.arjuna.JavaArjunaLite.Common.LockStoreException;



public class BasicPersistentLockStore extends LockStoreImple
{

    /*
     * Ignore key as we can make use of the basic type information for
     * this type of store. Really only need it for shared memory.
     */
    
public BasicPersistentLockStore (String key)
    {
	if (lockStoreLocation == null)
	    lockStoreLocation = System.getProperty(JavaArjunaLiteNames.Implementation_LockStore_storeDir(), "/usr/local/JTSArjuna/LockStore");
	
	_key = lockStoreLocation;

	/*
	 * Use the ShadowingStore since it has file-level locking which
	 * we require. The default object store assumes locking is provided
	 * entirely by the object.
	 */
	
	_lockStore = new ObjectStore(JavaArjunaLiteNames.Implementation_ObjectStore_ShadowingStore(), _key);
    }

public InputObjectState read_state (Uid u, String tName) throws LockStoreException
    {
	try
	{
	    return _lockStore.read_committed(u, tName);
	}
	catch (ObjectStoreException e)
	{
	    throw(new LockStoreException("Persistent store error."));
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
	return JavaArjunaLiteNames.Implementation_LockStore_BasicPersistentLockStore();
    }

public static ClassName name ()
    {
	return JavaArjunaLiteNames.Implementation_LockStore_BasicPersistentLockStore();
    }    

public static final BasicPersistentLockStore create (Object[] param)
    {
	System.out.println("making persistent locks");
	
	if (param == null)
	    return null;
	
	String key = (String) param[0];

	return new BasicPersistentLockStore(key);
    }
    
private String _key;
private ObjectStore _lockStore;

private static String lockStoreLocation = null;
    
};
