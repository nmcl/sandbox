/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: JDBCStoreSetup.java,v 1.1.2.1.60.1 2001/08/09 10:36:57 nmcl Exp $
 */

package com.arjuna.JDBC2.Implementation.ObjectStore;

import com.arjuna.ArjunaCore.ArjunaCoreNames;
import com.arjuna.ArjunaCore.Implementation.ObjectStoreImple;
import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaCommon.Base.InventoryElement;

public class JDBCStoreSetup implements InventoryElement
{

public JDBCStoreSetup ()
    {
    }

public synchronized Object createVoid ()
    {
	return JDBCStore.create();
    }
    
public synchronized Object createResources (Object[] param)
    {
	return JDBCStore.create(param);
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
	return ArjunaCoreNames.Implementation_ObjectStore_JDBCStore();
    }

}
