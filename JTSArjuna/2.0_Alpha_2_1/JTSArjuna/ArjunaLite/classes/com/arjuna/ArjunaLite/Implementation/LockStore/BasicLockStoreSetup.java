/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: BasicLockStoreSetup.java,v 1.1 2000/02/25 14:04:05 nmcl Exp $
 */

package com.arjuna.ArjunaLite.Implementation.LockStore;

import com.arjuna.ArjunaLite.ArjunaLiteNames;
import com.arjuna.ArjunaCommon.Common.ClassName;
import com.arjuna.ArjunaCommon.Common.ObjectName;
import com.arjuna.ArjunaCommon.Base.InventoryElement;

public class BasicLockStoreSetup implements InventoryElement
{

public BasicLockStoreSetup ()
    {
    }

public synchronized Object createVoid ()
    {
	return null;
    }
    
public synchronized Object createResources (Object[] param)
    {
	return BasicLockStore.create(param);
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
	return ArjunaLiteNames.Implementation_LockStore_BasicLockStore();
    }
    
}
