/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id$
 */

package com.arjuna.JavaArjuna.ObjectStore;

/*
 * Maintain the types of ObjectStore implementations.
 */

public class ObjectStoreType
{
    public static String SHADOWING_STORE = "ShadowingStore";
    public static String SHADOWNOFILELOCK_STORE = "ShadowNoFileLockStore";
    public static String ACTION_STORE = "ActionStore";
    public static String FILESYSTEM_STORE = "FileSystemStore";

    public static String DEFAULT_STORE = SHADOWNOFILELOCK_STORE;
    public static String DEFAULT_ACTIONSTORE_TYPE = ACTION_STORE;

    public static final ObjectStoreImple create (String type, String location)
    {
	if (SHADOWING_STORE.equals(type))
	    return new ShadowingStore(location);
	else
	{
	    if (SHADOWNOFILELOCK_STORE.equals(type))
		return new ShadowNoFileLockStore(location);
	    else
	    {
		if (ACTION_STORE.equals(type))
		    return new ActionStore(location);
		else
		{
		    if (FILESYSTEM_STORE.equals(type))
			return new FileSystemStore(location);
		    else
		    {
			System.err.println("ERROR - unknown ObjectStore type: "+type);
		    
			return null;
		    }
		}
	    }
	}
    }
};
