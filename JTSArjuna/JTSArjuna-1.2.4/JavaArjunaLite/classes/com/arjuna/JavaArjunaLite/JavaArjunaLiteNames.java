/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: JavaArjunaLiteNames.java,v 1.7.2.1 1999/02/22 10:32:00 nmcl Exp $
 */

package com.arjuna.JavaArjunaLite;

import com.arjuna.JavaGandiva.Common.ClassName;
import com.arjuna.JavaArjunaLite.Atomic.RecordType;

public class JavaArjunaLiteNames
{

public static ClassName Implementation_AbstractRecord_PersistenceRecord ()
    {
	return new ClassName(new String("AR"+RecordType.PERSISTENCE));
    }

public static ClassName Implementation_AbstractRecord_CadaverRecord ()
    {
	return new ClassName(new String("AR"+RecordType.CADAVER));
    }

public static ClassName Implementation_LockStore_defaultStore ()
    {
	return JavaArjunaLiteNames.Implementation_LockStore_BasicLockStore();
    }
    
public static ClassName Implementation_LockStore_BasicLockStore ()
    {
	return new ClassName("BasicLockStore");
    }

public static ClassName Implementation_LockStore_BasicPersistentLockStore ()
    {
	return new ClassName("BasicPersistentLockStore");
    }

public static String Implementation_LockStore_storeDir ()
    {
	return new String("LOCKSTORE_DIR");
    }

public static ClassName Implementation_Semaphore_BasicSemaphore ()
    {
	return new ClassName("BasicSemaphore");
    }

public static ClassName Implementation_ObjectStore_defaultStore ()
    {
	return JavaArjunaLiteNames.Implementation_ObjectStore_ShadowNoFileLockStore();
    }
    
public static ClassName Implementation_ObjectStore_defaultActionStore ()
    {
	return JavaArjunaLiteNames.Implementation_ObjectStore_ActionStore();
    }
    
public static ClassName Implementation_ObjectStore_ShadowingStore ()
    {
	return new ClassName(new String("SHDWSTR"));
    }

public static ClassName Implementation_ObjectStore_ShadowNoFileLockStore ()
    {
	return new ClassName(new String("SHDWNOFLSTR"));
    }

public static ClassName Implementation_ObjectStore_FileSystemStore ()
    {
	return new ClassName(new String("FSSTR"));
    }    

public static ClassName Implementation_ObjectStore_ActionStore ()
    {
	return new ClassName(new String("ACTSTR"));
    }

public static ClassName Implementation_ObjectStore_ReplicatedStore ()
    {
	return new ClassName(new String("REPSTR"));
    }

public static ClassName Implementation_ObjectStore_VolatileStore ()
    {
	return new ClassName(new String("VOLSTR"));
    }

public static String Implementation_ObjectStore_storeRoot ()
    {
	return new String("LOCALOSROOT");
    }

public static String Implementation_ObjectStore_storeDir ()
    {
	return new String("OBJECTSTORE_DIR");
    }

public static String Implementation_ObjectStore_doSync ()
    {
	return new String("OBJECTSTORE_SYNC");
    }
    
public static String Interface_LockStore_singleStoreType ()
    {
	return "SINGLE_LOCKSTORE";
    }

public static String Interface_LockStore_multipleStoreType ()
    {
	return "MULTIPLE_LOCKSTORE";
    }    

public static String Interface_ObjectStore_storeType ()
    {
	return "OBJECTSTORE_TYPE";
    }

public static String BasicAction_actionStore ()
    {
	return "ACTION_STORE";
    }
    
public static String StateManager_remoteState ()
    {
	return "RMTSTRE";
    }

public static String StateManager_useStoreLocation ()
    {
	return "USESL";
    }

public static String StateManager_numberOfStores ()
    {
	return "NUMBSTRES";
    }

public static String StateManager_objectModel ()
    {
	return "OBJMDL";
    }

public static String LockManager_remoteManager ()
    {
	return "RMTLCKMGR";
    }
    
};
