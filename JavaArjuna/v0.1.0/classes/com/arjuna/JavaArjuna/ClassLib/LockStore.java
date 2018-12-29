/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LockStore.java,v 1.6 1998/07/06 13:26:51 nmcl Exp $
 */

package com.arjuna.JavaArjuna.ClassLib;

import com.arjuna.JavaArjuna.ClassLib.ObjectModel;
import com.arjuna.JavaArjuna.Common.LockStoreException;
import com.arjuna.JavaArjuna.ClassLib.LockStoreImple;
import com.arjuna.JavaArjuna.Common.*;

public class LockStore
{

    /*
     * Default for single lock store type.
     */
    
public LockStore (String param)
    {
	Object[] resources = new Object[1];
	resources[0] = param;

	/*
	 * https://github.com/nmcl/sandbox/issues/66
	if (!singleCheck)
	{
	    singleLockStoreType = new ClassName(System.getProperty(JavaArjunaLiteNames.Interface_LockStore_singleStoreType(),
								   JavaArjunaLiteNames.Implementation_LockStore_defaultStore().stringForm()));
	    singleCheck = true;
	}

	Object ptr = Inventory.inventory().createResources(singleLockStoreType, resources);
	
	if (ptr instanceof LockStoreImple)
	    _imple = (LockStoreImple) ptr;
	else
	    _imple = null;
	*/
    }

    /*
     * https://github.com/nmcl/sandbox/issues/66
public LockStore (ClassName typeName, String param)
    {
	Object[] resources = new Object[1];
	resources[0] = param;
	
	Object ptr = Inventory.inventory().createResources(typeName, resources);

	if (ptr instanceof LockStoreImple)
	    _imple = (LockStoreImple) ptr;
	else
	    _imple = null;
    }
    */

public LockStore (Object[] param)
    {
	if (param.length == 3)
	{
	    /*
	     * https://github.com/nmcl/sandbox/issues/66
	    ClassName typeName = (ClassName) param[0];
	    */

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
		    /*
		     *  https://github.com/nmcl/sandbox/issues/66
		    type = System.getProperty(JavaArjunaLiteNames.Interface_LockStore_singleStoreType(), null);
		    */
		    singleCheck = true;

		    /*
		     * https://github.com/nmcl/sandbox/issues/66
		    if (type != null)
			singleLockStoreType = new ClassName(type);
		    */
		}

		/*
		 *  https://github.com/nmcl/sandbox/issues/66
		if (singleLockStoreType != null)
		    typeName = singleLockStoreType;
		*/
	    }
	    else
	    {
		if (!multipleCheck)
		{
		    /*
		     *  https://github.com/nmcl/sandbox/issues/66
		    type = System.getProperty(JavaArjunaLiteNames.Interface_LockStore_multipleStoreType(), null);
		    */
		    multipleCheck = true;

		    /*
		     * https://github.com/nmcl/sandbox/issues/66
		    if (type != null)
			multipleLockStoreType = new ClassName(type);
		    */
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

    /*
     * https://github.com/nmcl/sandbox/issues/75
public void finalize ()
    {
	_imple = null;
    }
    */
    
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

    /*
     * https://github.com/nmcl/sandbox/issues/66
public ClassName className ()
    {
	return ((_imple != null) ? _imple.className() : ClassName.invalid());
    }
    */
    
private LockStoreImple _imple;

    /*
     * https://github.com/nmcl/sandbox/issues/66
private static ClassName singleLockStoreType = null;
private static ClassName multipleLockStoreType = null;
    */
private static boolean singleCheck = false;
private static boolean multipleCheck = false;
 
    static 
    {
	if (!Implementations.added())
	    Implementations.initialise();
    }
    
}

