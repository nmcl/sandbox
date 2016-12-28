/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ArjunaLiteNames.java,v 1.1 2000/02/25 14:03:37 nmcl Exp $
 */

package com.arjuna.ArjunaLite;

import com.arjuna.ArjunaCommon.Common.ClassName;
import com.arjuna.ArjunaLite.Atomic.RecordType;

public class ArjunaLiteNames
{

public static String StateManager_remoteState ()
    {
	return "REMOTESTORE";
    }

public static String StateManager_useStoreLocation ()
    {
	return "USESTORELOCATION";
    }

public static String StateManager_numberOfStores ()
    {
	return "NUMBER_OF_STORES";
    }

public static String StateManager_objectModel ()
    {
	return "OBJECTMODEL";
    }

public static String LockManager_remoteManager ()
    {
	return "REMOTELOCKMANAGER";
    }
    
public static ClassName Implementation_AbstractRecord_PersistenceRecord ()
    {
	return new ClassName(new String("PersistenceRecord"));
    }

public static ClassName Implementation_AbstractRecord_CadaverRecord ()
    {
	return new ClassName(new String("CadaverRecord"));
    }

public static ClassName Implementation_AbstractRecord_CadaverRecord_DisposeRecord ()
    {
	return new ClassName(new String("DisposeRecord"));
    }    

public static ClassName Implementation_LockStore_defaultStore ()
    {
	return ArjunaLiteNames.Implementation_LockStore_BasicLockStore();
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
	return ArjunaLiteNames.Implementation_ObjectStore_ShadowNoFileLockStore();
    }
    
public static ClassName Implementation_ObjectStore_defaultActionStore ()
    {
	return ArjunaLiteNames.Implementation_ObjectStore_ActionStore();
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

public static ClassName Implementation_ObjectStore_MappedStore ()
    {
	return new ClassName(new String("MappedStore"));
    }

public static ClassName Implementation_ObjectStore_SingleTypeMappedStore ()
    {
	return new ClassName(new String("SingleTypeMappedStore"));
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
