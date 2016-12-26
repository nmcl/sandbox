/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: BasicPersistentLockStoreSetup.java,v 1.1.4.1 2000/10/16 16:00:28 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Implementation.LockStore;

import com.arjuna.ArjunaCore.ArjunaCoreNames;
import com.arjuna.ArjunaCommon.Common.ClassName;
import com.arjuna.ArjunaCommon.Common.ObjectName;
import com.arjuna.ArjunaCommon.Base.InventoryElement;

public class BasicPersistentLockStoreSetup implements InventoryElement
{

public BasicPersistentLockStoreSetup ()
    {
    }

public synchronized Object createVoid ()
    {
	return null;
    }

public synchronized Object createResources (Object[] param)
    {
	return BasicPersistentLockStore.create(param);
    }

public synchronized Object createClassName (ClassName className)
    {
	return null;
    }
    
public synchronized Object createObjectName (ObjectName objectName)
    {
	return null;
    }
    
public synchronized Object createClassNameResources (ClassName className, Object[] resources)
    {
	return null;
    }
    
public synchronized Object createObjectNameResources (ObjectName objectName, Object[] resources)
    {
	return null;
    }
    
public ClassName className ()
    {
	return ArjunaCoreNames.Implementation_LockStore_BasicPersistentLockStore();
    }
    
}
