/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: LockStore.java,v 1.1 2000/02/25 14:04:17 nmcl Exp $
 */

package com.arjuna.ArjunaLite.Interface;

import com.arjuna.ArjunaLite.ArjunaLiteNames;
import com.arjuna.ArjunaLite.Atomic.ObjectModel;
import com.arjuna.ArjunaLite.Common.*;
import com.arjuna.ArjunaLite.Implementation.Implementations;
import com.arjuna.ArjunaLite.Implementation.LockStoreImple;
import com.arjuna.ArjunaCommon.Interface.Inventory;
import com.arjuna.ArjunaCommon.Common.*;

public class LockStore
{

    /*
     * Default for single lock store type.
     */
    
public LockStore (String param)
    {
	Object[] resources = new Object[1];
	resources[0] = param;

	if (!singleCheck)
	{
	    singleLockStoreType = new ClassName(PropertyManager.getProperty(ArjunaLiteEnvironment.SINGLE_LOCKSTORE,
									    ArjunaLiteNames.Implementation_LockStore_defaultStore().stringForm()));
	    singleCheck = true;
	}

	Object ptr = Inventory.inventory().createResources(singleLockStoreType, resources);

	if (ptr instanceof LockStoreImple)
	    _imple = (LockStoreImple) ptr;
	else
	    _imple = null;
    }

public LockStore (ClassName typeName, String param)
    {
	if (typeName == null)
	{
	    if (!singleCheck)
	    {
		singleLockStoreType = new ClassName(PropertyManager.getProperty(ArjunaLiteEnvironment.SINGLE_LOCKSTORE,
										ArjunaLiteNames.Implementation_LockStore_defaultStore().stringForm()));
		singleCheck = true;
	    }

	    typeName = singleLockStoreType;
	}
	    
	Object[] resources = new Object[1];
	resources[0] = param;
	
	Object ptr = Inventory.inventory().createResources(typeName, resources);

	if (ptr instanceof LockStoreImple)
	    _imple = (LockStoreImple) ptr;
	else
	    _imple = null;
    }

public LockStore (Object[] param)
    {
	if (param.length == 3)
	{
	    ClassName typeName = (ClassName) param[0];
	    int modelType = ((Integer) param[1]).intValue();
	    Object[] resources = new Object[1];

	    resources[0] = param[2];

	    /*
	     * Assume anything in the property field overrides anything given
	     * by the application.
	     */

	    String type = null;
	    
	    if (modelType == ObjectModel.SINGLE)
	    {
		if (!singleCheck)
		{
		    type = PropertyManager.getProperty(ArjunaLiteEnvironment.SINGLE_LOCKSTORE);
		    singleCheck = true;

		    if (type != null)
			singleLockStoreType = new ClassName(type);
		}

		if (singleLockStoreType != null)
		    typeName = singleLockStoreType;
	    }
	    else
	    {
		if (!multipleCheck)
		{
		    type = PropertyManager.getProperty(ArjunaLiteEnvironment.MULTIPLE_LOCKSTORE, null);
		    multipleCheck = true;

		    if (type != null)
			multipleLockStoreType = new ClassName(type);
		}

		if (multipleLockStoreType != null)
		    typeName = multipleLockStoreType;
	    }

	    Object ptr = Inventory.inventory().createResources(typeName, resources);

	    if (ptr instanceof LockStoreImple)
		_imple = (LockStoreImple) ptr;
	    else
		_imple = null;
	    
	}
	else
	    _imple = null;
    }
	    
public void finalize ()
    {
	_imple = null;
    }
    
public InputObjectState read_state (Uid u, String tName) throws LockStoreException
    {
	return ((_imple != null) ? _imple.read_state(u, tName) : null);
    }
    
public boolean remove_state (Uid u, String tname)
    {
	return ((_imple != null) ? _imple.remove_state(u, tname) : false);
    }
    
public boolean write_committed (Uid u, String tName, OutputObjectState state)
    {
	return ((_imple != null) ? _imple.write_committed(u, tName, state) : false);
    }

public ClassName className ()
    {
	return ((_imple != null) ? _imple.className() : ClassName.invalid());
    }
    
private LockStoreImple _imple;

private static ClassName singleLockStoreType = null;
private static ClassName multipleLockStoreType = null;
private static boolean singleCheck = false;
private static boolean multipleCheck = false;
 
    static 
    {
	if (!Implementations.added())
	    Implementations.initialise();
    }
    
}

