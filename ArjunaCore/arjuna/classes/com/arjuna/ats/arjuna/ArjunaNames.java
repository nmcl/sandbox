/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ArjunaNames.java,v 1.2 2003/06/26 10:03:44 nmcl Exp $
 */

package com.arjuna.ats.arjuna;

import com.arjuna.ats.arjuna.gandiva.ClassName;
import com.arjuna.ats.arjuna.coordinator.RecordType;

/**
 * This class contains the ClassNames and ObjectName attributes that
 * may be used by implementations within this module.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ArjunaNames.java,v 1.2 2003/06/26 10:03:44 nmcl Exp $
 * @since 1.0.
 * @see com.arjuna.ats.arjuna.gandiva.ClassName
 * @see com.arjuna.ats.arjuna.gandiva.ObjectName
 */

public class ArjunaNames
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

    public static String ObjectStore_implementationObjectName ()
    {
	return "OS_OBJECTNAME";
    }

    public static String Implementation_ObjectStore_JDBC_tableName ()
    {
	return "TABLE_NAME";
    }

    public static String Implementation_ObjectStore_JDBC_url ()
    {
	return "URL";
    }

    public static String Implementation_ObjectStore_JDBC_dropTable ()
    {
	return "DROP_TABLE";
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

    public static ClassName Implementation_Semaphore_BasicSemaphore ()
    {
	return new ClassName("BasicSemaphore");
    }

    public static ClassName Implementation_ObjectStore_defaultStore ()
    {
	return ArjunaNames.Implementation_ObjectStore_ShadowNoFileLockStore();
    }
    
    public static ClassName Implementation_ObjectStore_defaultActionStore ()
    {
	return ArjunaNames.Implementation_ObjectStore_HashedActionStore();
    }
    
    public static ClassName Implementation_ObjectStore_ShadowingStore ()
    {
	return new ClassName("ShadowingStore");
    }

    public static ClassName Implementation_ObjectStore_ShadowNoFileLockStore ()
    {
	return new ClassName("ShadowNoFileLockStore");
    }

    public static ClassName Implementation_ObjectStore_FileSystemStore ()
    {
	return new ClassName("FileSystemStore");
    }    

    public static ClassName Implementation_ObjectStore_ActionStore ()
    {
	return new ClassName("ActionStore");
    }

    public static ClassName Implementation_ObjectStore_ActionLogStore ()
    {
	return new ClassName("ActionLogStore");
    }    

    public static ClassName Implementation_ObjectStore_ReplicatedStore ()
    {
	return new ClassName("ReplicatedStore");
    }

    public static ClassName Implementation_ObjectStore_VolatileStore ()
    {
	return new ClassName("VolatileStore");
    }

    public static ClassName Implementation_ObjectStore_FragmentedStore ()
    {
	return new ClassName("FragmentedStore");
    }

    public static ClassName Implementation_ObjectStore_HashedStore ()
    {
	return new ClassName("HashedStore");
    }

    public static ClassName Implementation_ObjectStore_HashedActionStore ()
    {
	return new ClassName("HashedActionStore");
    }

    public static ClassName Implementation_ObjectStore_MappedStore ()
    {
	return new ClassName("MappedStore");
    }

    public static ClassName Implementation_ObjectStore_SingleTypeMappedStore ()
    {
	return new ClassName("SingleTypeMappedStore");
    }

    public static ClassName Implementation_ObjectStore_JDBCStore ()
    {
	return new ClassName("JDBCStore");
    }

    public static ClassName Implementation_ObjectStore_JDBCActionStore ()
    {
	return new ClassName("JDBCActionStore");
    }

    public static ClassName Implementation_ObjectStore_CacheStore ()
    {
	return new ClassName("CacheStore");
    }

    public static ClassName Implementation_ObjectStore_UserDef0Store ()
    {
	return new ClassName("UserDef0Store");
    }

    public static ClassName Implementation_ObjectStore_UserDef1Store ()
    {
	return new ClassName("UserDef1Store");
    }

    public static ClassName Implementation_ObjectStore_UserDef2Store ()
    {
	return new ClassName("UserDef2Store");
    }

    public static ClassName Implementation_ObjectStore_UserDef3Store ()
    {
	return new ClassName("UserDef3Store");
    }

    public static ClassName Implementation_ObjectStore_UserDef4Store ()
    {
	return new ClassName("UserDef4Store");
    }

    public static ClassName Implementation_NameService_JNS ()
    {
	return new ClassName("JNSNameServiceImple");
    }
    
    public static ClassName Implementation_NameService_PNS ()
    {
	return new ClassName("PNSNameServiceImple");
    }

    public static ClassName Implementation_Inventory_StaticInventory ()
    {
	return new ClassName("StaticInventoryImple");
    }

    public static ClassName Implementation_Inventory_DynamicInventory ()
    {
	return new ClassName("DynamicInventoryImple");
    }

    public static ClassName Interface_Inventory ()
    {
	return new ClassName("Inventory");
    }

    public static ClassName Interface_NameService ()
    {
	return new ClassName("NameService");
    }
    
};
