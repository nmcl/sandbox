/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BasicLockStoreSetup.java,v 1.5 1998/07/06 13:26:41 nmcl Exp $
 */

package com.arjuna.JavaArjunaLite.Implementation.LockStore;

import com.arjuna.JavaArjunaLite.JavaArjunaLiteNames;
import com.arjuna.JavaGandiva.Common.ClassName;
import com.arjuna.JavaGandiva.Common.ObjectName;
import com.arjuna.JavaGandiva.Base.InventoryElement;

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
	return JavaArjunaLiteNames.Implementation_LockStore_BasicLockStore();
    }
    
}
