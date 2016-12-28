/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ShadowNoFileLockStore.java,v 1.1 2000/02/25 14:04:11 nmcl Exp $
 */

package com.arjuna.ArjunaLite.Implementation.ObjectStore;

import com.arjuna.ArjunaLite.ArjunaLiteNames;
import com.arjuna.ArjunaCommon.Common.ClassName;
import java.io.File;

/*
 * Almost the same as the ShadowingStore implementation, but
 * assumes all concurrency control is provided by the object.
 * Therefore, there is no need to set/release locks on the
 * file representation in the object store. Saves time.
 */

public class ShadowNoFileLockStore extends ShadowingStore
{

public int typeIs ()
    {
	return ObjectStoreType.SHADOWNOFILELOCK;
    }
    
public ClassName className ()
    {
	return ArjunaLiteNames.Implementation_ObjectStore_ShadowNoFileLockStore();
    }

public static ClassName name ()
    {
	return ArjunaLiteNames.Implementation_ObjectStore_ShadowNoFileLockStore();
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
