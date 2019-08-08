/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ShadowNoFileLockStore.java,v 1.1.4.1.2.1.2.2.2.1 2000/12/21 11:21:10 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Implementation.ObjectStore;

import com.arjuna.ArjunaCore.ArjunaCoreNames;
import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaCore.Interface.ObjectStore;
import java.io.File;

/**
 * Almost the same as the ShadowingStore implementation, but
 * assumes all concurrency control is provided by the object.
 * Therefore, there is no need to set/release locks on the
 * file representation in the object store. Saves time.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ShadowNoFileLockStore.java,v 1.1.4.1.2.1.2.2.2.1 2000/12/21 11:21:10 nmcl Exp $
 * @since JTS 1.0.
 */

public class ShadowNoFileLockStore extends ShadowingStore
{

public int typeIs ()
    {
	return ObjectStoreType.SHADOWNOFILELOCK;
    }
    
public ClassName className ()
    {
	return ArjunaCoreNames.Implementation_ObjectStore_ShadowNoFileLockStore();
    }

public static ClassName name ()
    {
	return ArjunaCoreNames.Implementation_ObjectStore_ShadowNoFileLockStore();
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
	Integer shareStatus = (Integer) param[1];
	int ss = ObjectStore.OS_UNSHARED;
	
	if (shareStatus != null)
	{
	    try
	    {
		ss = shareStatus.intValue();
	    }
	    catch (Exception e)
	    {
		ErrorStream.warning().println("ShadowNoFileLockStore.create caught: "+e);
	    }
	}

	return new ShadowNoFileLockStore(location, ss);
    }
	    
protected ShadowNoFileLockStore (String locationOfStore)
    {
	this(locationOfStore, ObjectStore.OS_UNSHARED);
    }
    
protected ShadowNoFileLockStore (String locationOfStore, int shareStatus)
    {
	super(locationOfStore, shareStatus);

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PROTECTED,
						 FacilityCode.FAC_OBJECT_STORE, "ShadowNoFileLockStore.ShadowNoFileLockStore("+locationOfStore+")");
	}
    }
    
protected ShadowNoFileLockStore ()
    {
	this(ObjectStore.OS_UNSHARED);
    }
	
protected ShadowNoFileLockStore (int shareStatus)
    {
	super(shareStatus);

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PROTECTED,
						 FacilityCode.FAC_OBJECT_STORE, "ShadowNoFileLockStore.ShadowNoFileLockStore()");
	}
    }

    /**
     * Override the default lock/unlock implementations to do nothing.
     */

protected synchronized boolean lock (File fd, int lmode, boolean create)
    {
	return true;
    }

protected synchronized boolean unlock (File fd)
    {
	return true;
    }
    
}
