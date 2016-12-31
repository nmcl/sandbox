/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ShadowNoFileLockStoreSetup.java,v 1.1 2000/02/25 14:04:12 nmcl Exp $
 */

package com.arjuna.ArjunaLite.Implementation.ObjectStore;

import com.arjuna.ArjunaLite.ArjunaLiteNames;
import com.arjuna.ArjunaLite.Implementation.ObjectStoreImple;
import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaCommon.Base.InventoryElement;

public class ShadowNoFileLockStoreSetup implements InventoryElement
{

public ShadowNoFileLockStoreSetup ()
    {
    }

public synchronized Object createVoid ()
    {
	return (ShadowNoFileLockStore) ShadowNoFileLockStore.create();
    }
    
public synchronized Object createResources (Object[] param)
    {
	return (ShadowNoFileLockStore) ShadowNoFileLockStore.create(param);
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
	return ArjunaLiteNames.Implementation_ObjectStore_ShadowNoFileLockStore();
    }
    
}
