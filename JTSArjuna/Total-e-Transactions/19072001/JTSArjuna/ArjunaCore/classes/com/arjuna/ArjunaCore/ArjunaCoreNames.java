/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ArjunaCoreNames.java,v 1.1.4.1.2.1.2.2.2.1 2000/12/21 11:20:58 nmcl Exp $
 */

package com.arjuna.ArjunaCore;

import com.arjuna.ArjunaCommon.Common.ClassName;
import com.arjuna.ArjunaCore.Atomic.RecordType;

/**
 * This class contains the ClassNames and ObjectName attributes that
 * may be used by implementations within this module.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ArjunaCoreNames.java,v 1.1.4.1.2.1.2.2.2.1 2000/12/21 11:20:58 nmcl Exp $
 * @since JTS 1.0.
 * @see com.arjuna.ArjunaCommon.Common.ClassName
 * @see com.arjuna.ArjunaCommon.Common.ObjectName
 */

public class ArjunaCoreNames
{

public static String StateManager_objectModel ()
    {
	return "OBJECTMODEL";
    }

public static String StateManager_uid ()
    {
	return "UID";
    }

public static String StateManager_objectType ()
    {
	return "OBJECT_TYPE";
    }
    
public static ClassName Implementation_AbstractRecord_PersistenceRecord ()
    {
	return new ClassName("RecordType.Persistence");
    }

public static ClassName Implementation_AbstractRecord_CadaverRecord ()
    {
	return new ClassName("RecordType.Cadaver");
    }

public static ClassName Implementation_AbstractRecord_CadaverRecord_DisposeRecord ()
    {
	return new ClassName("RecordType.Dispose");
    }    

public static ClassName Implementation_LockStore_defaultStore ()
    {
	return ArjunaCoreNames.Implementation_LockStore_BasicLockStore();
    }
    
public static ClassName Implementation_LockStore_BasicLockStore ()
    {
	return new ClassName("BasicLockStore");
    }

public static ClassName Implementation_LockStore_BasicPersistentLockStore ()
    {
	return new ClassName("BasicPersistentLockStore");
    }

public static ClassName Implementation_Semaphore_BasicSemaphore ()
    {
	return new ClassName("BasicSemaphore");
    }

public static ClassName Implementation_ObjectStore_defaultStore ()
    {
	return ArjunaCoreNames.Implementation_ObjectStore_ShadowNoFileLockStore();
    }
    
public static ClassName Implementation_ObjectStore_defaultActionStore ()
    {
	return ArjunaCoreNames.Implementation_ObjectStore_HashedActionStore();
    }
    
public static ClassName Implementation_ObjectStore_ShadowingStore ()
    {
	return new ClassName(new String("ShadowingStore"));
    }

public static ClassName Implementation_ObjectStore_ShadowNoFileLockStore ()
    {
	return new ClassName(new String("ShadowNoFileLockStore"));
    }

public static ClassName Implementation_ObjectStore_FileSystemStore ()
    {
	return new ClassName(new String("FileSystemStore"));
    }    

public static ClassName Implementation_ObjectStore_ActionStore ()
    {
	return new ClassName(new String("ActionStore"));
    }

public static ClassName Implementation_ObjectStore_ActionLogStore ()
    {
	return new ClassName(new String("ActionLogStore"));
    }    

public static ClassName Implementation_ObjectStore_ReplicatedStore ()
    {
	return new ClassName(new String("ReplicatedStore"));
    }

public static ClassName Implementation_ObjectStore_VolatileStore ()
    {
	return new ClassName(new String("VolatileStore"));
    }

public static ClassName Implementation_ObjectStore_FragmentedStore ()
    {
	return new ClassName(new String("FragmentedStore"));
    }

public static ClassName Implementation_ObjectStore_HashedStore ()
    {
	return new ClassName(new String("HashedStore"));
    }

public static ClassName Implementation_ObjectStore_HashedActionStore ()
    {
	return new ClassName(new String("HashedActionStore"));
    }

public static ClassName Implementation_ObjectStore_MappedStore ()
    {
	return new ClassName(new String("MappedStore"));
    }

public static ClassName Implementation_ObjectStore_SingleTypeMappedStore ()
    {
	return new ClassName(new String("SingleTypeMappedStore"));
    }

public static ClassName Implementation_ObjectStore_JDBCStore ()
    {
	return new ClassName(new String("JDBCStore"));
    }

public static ClassName Implementation_ObjectStore_JDBCActionStore ()
    {
	return new ClassName(new String("JDBCActionStore"));
    }

public static ClassName Implementation_ObjectStore_UserDef0Store ()
    {
	return new ClassName(new String("UserDef0Store"));
    }

public static ClassName Implementation_ObjectStore_UserDef1Store ()
    {
	return new ClassName(new String("UserDef1Store"));
    }

public static ClassName Implementation_ObjectStore_UserDef2Store ()
    {
	return new ClassName(new String("UserDef2Store"));
    }

public static ClassName Implementation_ObjectStore_UserDef3Store ()
    {
	return new ClassName(new String("UserDef3Store"));
    }

public static ClassName Implementation_ObjectStore_UserDef4Store ()
    {
	return new ClassName(new String("UserDef4Store"));
    }
    
};
