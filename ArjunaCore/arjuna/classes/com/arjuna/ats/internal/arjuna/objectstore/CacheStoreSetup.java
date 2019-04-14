/*
 * Copyright (C) 2003,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: CacheStoreSetup.java,v 1.1 2003/06/26 10:03:45 nmcl Exp $
 */

package com.arjuna.ats.internal.arjuna.objectstore;

import com.arjuna.ats.arjuna.ArjunaNames;
import com.arjuna.ats.arjuna.objectstore.ObjectStoreImple;
import com.arjuna.ats.arjuna.common.*;
import com.arjuna.ats.arjuna.gandiva.*;
import com.arjuna.ats.arjuna.gandiva.inventory.*;

public class CacheStoreSetup implements InventoryElement
{

public CacheStoreSetup ()
    {
    }

public synchronized Object createVoid ()
    {
	return CacheStore.create();
    }
    
public synchronized Object createResources (Object[] param)
    {
	return CacheStore.create(param);
    }
    
public synchronized Object createClassName (ClassName className)
    {
	return null;
    }
    
public synchronized Object createObjectName (ObjectName objectName)
    {
	return CacheStore.create(objectName);
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
	return ArjunaNames.Implementation_ObjectStore_CacheStore();
    }

}
