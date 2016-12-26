/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: BasicPersistentLockStore.java,v 1.1.4.1.2.1.4.1.4.1 2001/01/26 16:07:52 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Implementation.LockStore;

import com.arjuna.ArjunaCore.ArjunaCoreNames;
import com.arjuna.ArjunaCore.Implementation.LockStoreImple;
import com.arjuna.ArjunaCore.Interface.ObjectStore;
import com.arjuna.ArjunaCore.Common.ArjunaCoreEnvironment;
import com.arjuna.ArjunaCore.Common.Configuration;
import com.arjuna.ArjunaCommon.Common.*;

import com.arjuna.ArjunaCore.Common.ObjectStoreException;
import com.arjuna.ArjunaCore.Common.LockStoreException;

/**
 * An implementation of the lock store which saves locks into files
 * on the local machine. Thus, multiple JVMs may share locks by reading
 * and writing to the same files.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: BasicPersistentLockStore.java,v 1.1.4.1.2.1.4.1.4.1 2001/01/26 16:07:52 nmcl Exp $
 * @since JTS 1.0.
 */

public class BasicPersistentLockStore extends LockStoreImple
{

    /*
     * Ignore key as we can make use of the basic type information for
     * this type of store. Really only need it for shared memory.
     */
    
public BasicPersistentLockStore (String key)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_LOCK_STORE, "BasicPersistentLockStore.BasicPersistentLockStore("+key+")");
	}

	if (lockStoreLocation == null)
	    lockStoreLocation = PropertyManager.getProperty(ArjunaCoreEnvironment.LOCKSTORE_DIR,
							    com.arjuna.ArjunaCore.Common.Configuration.lockStoreRoot());
	
	_key = lockStoreLocation;

	/*
	 * Use the ShadowingStore since it has file-level locking which
	 * we require. The default object store assumes locking is provided
	 * entirely by the object.
	 */
	
	_lockStore = new ObjectStore(ArjunaCoreNames.Implementation_ObjectStore_ShadowingStore(), _key);
    }

public InputObjectState read_state (Uid u, String tName) throws LockStoreException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_LOCK_STORE, "BasicPersistentLockStore.read_state("+u+", "+tName+")");
	}

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
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_LOCK_STORE, "BasicPersistentLockStore.remove_state("+u+", "+tName+")");
	}

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
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_LOCK_STORE, "BasicPersistentLockStore.write_committed("+u+", "+tName+", "+state+")");
	}

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
	return ArjunaCoreNames.Implementation_LockStore_BasicPersistentLockStore();
    }

public static ClassName name ()
    {
	return ArjunaCoreNames.Implementation_LockStore_BasicPersistentLockStore();
    }    

public static final BasicPersistentLockStore create (Object[] param)
    {
	if (param == null)
	    return null;
	
	String key = (String) param[0];

	return new BasicPersistentLockStore(key);
    }
    
private String      _key;
private ObjectStore _lockStore;

private static String lockStoreLocation = null;
    
}
