/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ShadowNoFileLockStore.java,v 1.5 1998/07/06 13:26:46 nmcl Exp $
 */

package com.arjuna.JavaArjuna.ObjectStore;

import java.io.File;

/*
 * Almost the same as the ShadowingStore implementation, but
 * assumes all concurrency control is provided by the object.
 * Therefore, there is no need to set/release locks on the
 * file representation in the object store. Saves time.
 */

public class ShadowNoFileLockStore extends ShadowingStore
{

public ClassName className ()
    {
	return JavaArjunaLiteNames.Implementation_ObjectStore_ShadowNoFileLockStore();
    }

public static ClassName name ()
    {
	return JavaArjunaLiteNames.Implementation_ObjectStore_ShadowNoFileLockStore();
    }    

    /*
     * Have to return as a ShadowingStore because of
     * inheritence.
     */
    
public static ShadowingStore create ()
    {
	return new ShadowNoFileLockStore("");
    }

public static ShadowingStore create (Object[] param)
    {
	if (param == null)
	    return null;

	String location = (String) param[0];

	return new ShadowNoFileLockStore(location);
    }
	    
protected ShadowNoFileLockStore (String locationOfStore)
    {
	super(locationOfStore);
    }
    
protected ShadowNoFileLockStore ()
    {
	super();
    }

protected synchronized boolean lock (File fd, int lmode, boolean create)
    {
	return true;
    }

protected synchronized boolean unlock (File fd)
    {
	return true;
    }
    
}
