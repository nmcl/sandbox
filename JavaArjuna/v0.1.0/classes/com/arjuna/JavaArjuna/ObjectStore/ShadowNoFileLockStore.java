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

public String className ()
    {
	return ObjectStoreType.SHADOWNOFILELOCK_STORE;
    }

public static String name ()
    {
	return ObjectStoreType.SHADOWNOFILELOCK_STORE;
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
