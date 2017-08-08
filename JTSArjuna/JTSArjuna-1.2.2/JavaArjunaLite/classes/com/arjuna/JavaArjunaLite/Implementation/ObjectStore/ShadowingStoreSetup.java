/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ShadowingStoreSetup.java,v 1.6 1998/07/06 13:26:48 nmcl Exp $
 */

package com.arjuna.JavaArjunaLite.Implementation.ObjectStore;

import com.arjuna.JavaArjunaLite.JavaArjunaLiteNames;
import com.arjuna.JavaArjunaLite.Implementation.ObjectStoreImple;
import com.arjuna.JavaGandiva.Common.*;
import com.arjuna.JavaGandiva.Base.InventoryElement;

public class ShadowingStoreSetup implements InventoryElement
{

public ShadowingStoreSetup ()
    {
    }

public synchronized Object createVoid ()
    {
	return ShadowingStore.create();
    }
    
public synchronized Object createResources (Object[] param)
    {
	return ShadowingStore.create(param);
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
	return JavaArjunaLiteNames.Implementation_ObjectStore_ShadowingStore();
    }

}
