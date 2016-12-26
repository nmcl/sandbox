/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: HashedActionStoreSetup.java,v 1.1.2.1 2000/12/13 14:35:22 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Implementation.ObjectStore;

import com.arjuna.ArjunaCore.ArjunaCoreNames;
import com.arjuna.ArjunaCore.Implementation.ObjectStoreImple;
import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaCommon.Base.InventoryElement;

public class HashedActionStoreSetup implements InventoryElement
{

public HashedActionStoreSetup ()
    {
    }

public synchronized Object createVoid ()
    {
	return HashedActionStore.create();
    }
    
public synchronized Object createResources (Object[] param)
    {
	return HashedActionStore.create(param);
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
	return ArjunaCoreNames.Implementation_ObjectStore_HashedActionStore();
    }

}
