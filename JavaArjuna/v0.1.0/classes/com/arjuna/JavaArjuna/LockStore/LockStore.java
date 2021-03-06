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

package com.arjuna.JavaArjuna.LockStore;

import com.arjuna.JavaArjuna.Environment;
import com.arjuna.JavaArjuna.ClassLib.ObjectModel;
import com.arjuna.JavaArjuna.Common.LockStoreException;
import com.arjuna.JavaArjuna.Common.*;

public class LockStore
{

    /*
     * Default for single lock store type.
     */

    /*
     * This dynamic binding is probably still more than was there in the original
     * version of JavaArjuna but I can't recall how it looked so anything that
     * wipes this out and starts from scratch is likely to be different anyway so
     * easier (quicker) to stay with something close to the earliest version of
     * JTSArjuna.
     */
    
public LockStore (String param)
    {
	if (!singleCheck)
	{
	    singleLockStoreType = System.getProperty(Environment.LOCKSTORE_TYPE, LockStoreType.BASIC_LOCKSTORE);
	    singleCheck = true;
	}

	_imple = new BasicLockStore(param);
    }

public LockStore (String typeName, String param)
    {
	if (LockStoreType.BASIC_LOCKSTORE.equals(typeName))
	    _imple = new BasicLockStore(param);
	else
	{
	    if (LockStoreType.BASICPERSISTENT_LOCKSTORE.equals(typeName))
		_imple = new BasicPersistentLockStore(param);
	    else
		_imple = null;
	}
    }

public LockStore (Object[] param)
    {
	if (param.length == 3)
	{
	    String typeName = (String) param[0];

	    int modelType = ((Integer) param[1]).intValue();
	    String key = (String) param[2];

	    /*
	     * Assume anything in the property field overrides anything given
	     * by the application.
	     */

	    String type = null;
	    
	    if (modelType == ObjectModel.SINGLE)
	    {
		if (!singleCheck)
		{
		    type = System.getProperty(Environment.LOCKSTORE_TYPE, null);
		    singleCheck = true;

		    if (type != null)
			singleLockStoreType = type;
		}

		if (singleLockStoreType != null)
		{
		    typeName = singleLockStoreType;

		    // just use the only implementation we have anyway!

		    _imple = new BasicLockStore(key);
		}
	    }
	    else
	    {
		if (!multipleCheck)
		{
		    type = System.getProperty(Environment.LOCKSTORE_TYPE, null);
		    multipleCheck = true;

		    if (type != null)
			multipleLockStoreType = type;
		}

		if (multipleLockStoreType != null)
		{
		    typeName = multipleLockStoreType;

		    // just use the only implementation we have anyway!

		    _imple = new BasicPersistentLockStore(key);
		}
	    }
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

public String className ()
    {
	return ((_imple != null) ? _imple.className() : null);
    }
    
private LockStoreImple _imple;

    /*
     * Not a typical way of doing this since LockStoreType has the values
     * but trying not to modify the code too much from the original. The
     * introduction of LockStoreType is already a deviation from the original.
     */
    
private static String singleLockStoreType = LockStoreType.BASIC_LOCKSTORE;
private static String multipleLockStoreType = LockStoreType.BASICPERSISTENT_LOCKSTORE;

private static boolean singleCheck = false;
private static boolean multipleCheck = false;
    
}

