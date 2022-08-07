/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: JDBCActionStoreSetup.java,v 1.1.2.2 2000/11/28 12:15:53 nmcl Exp $
 */

package com.arjuna.JDBC2.Implementation.ObjectStore;

import com.arjuna.ArjunaCore.ArjunaCoreNames;
import com.arjuna.ArjunaCore.Implementation.ObjectStoreImple;
import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaCommon.Base.InventoryElement;

public class JDBCActionStoreSetup implements InventoryElement
{

public JDBCActionStoreSetup ()
    {
	_theStore = null;
    }

public synchronized Object createVoid ()
    {
	if (_theStore == null)
	    _theStore = (JDBCActionStore) JDBCActionStore.create();

	return _theStore;
    }
    
public synchronized Object createResources (Object[] param)
    {
	if (_theStore == null)
	    _theStore = (JDBCActionStore) JDBCActionStore.create(param);

	return _theStore;
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
	return ArjunaCoreNames.Implementation_ObjectStore_JDBCActionStore();
    }

    // only a single instance for now.

private JDBCActionStore _theStore;
    
}
