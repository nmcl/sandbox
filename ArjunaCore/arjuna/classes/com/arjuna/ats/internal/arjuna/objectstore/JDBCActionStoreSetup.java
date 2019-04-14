/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: JDBCActionStoreSetup.java,v 1.1 2003/06/19 10:50:32 nmcl Exp $
 */

package com.arjuna.ats.internal.arjuna.objectstore;

import com.arjuna.ats.arjuna.ArjunaNames;
import com.arjuna.ats.arjuna.objectstore.ObjectStoreImple;
import com.arjuna.ats.arjuna.common.*;
import com.arjuna.ats.arjuna.gandiva.*;
import com.arjuna.ats.arjuna.gandiva.inventory.InventoryElement;

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
	return ArjunaNames.Implementation_ObjectStore_JDBCActionStore();
    }

    // only a single instance for now.

private JDBCActionStore _theStore;
    
}
