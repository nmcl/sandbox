/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ShadowNoFileLockStore.java,v 1.3 2003/08/11 15:04:19 nmcl Exp $
 */

package com.arjuna.ats.internal.arjuna.objectstore;

import com.arjuna.ats.arjuna.ArjunaNames;
import com.arjuna.ats.arjuna.common.*;
import com.arjuna.ats.arjuna.objectstore.ObjectStore;
import com.arjuna.ats.arjuna.objectstore.ObjectStoreType;
import com.arjuna.ats.arjuna.common.*;
import com.arjuna.ats.arjuna.state.*;

import com.arjuna.ats.arjuna.logging.tsLogger;
import com.arjuna.ats.arjuna.logging.FacilityCode;

import com.arjuna.common.util.logging.DebugLevel;
import com.arjuna.common.util.logging.VisibilityLevel;

import com.arjuna.ats.arjuna.gandiva.ClassName;
import com.arjuna.ats.arjuna.gandiva.ObjectName;
import com.arjuna.ats.arjuna.utils.FileLock;
import com.arjuna.ats.arjuna.utils.Utility;
import java.io.File;

/**
 * Almost the same as the ShadowingStore implementation, but
 * assumes all concurrency control is provided by the object.
 * Therefore, there is no need to set/release locks on the
 * file representation in the object store. Saves time.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ShadowNoFileLockStore.java,v 1.3 2003/08/11 15:04:19 nmcl Exp $
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
	return ArjunaNames.Implementation_ObjectStore_ShadowNoFileLockStore();
    }

public static ClassName name ()
    {
	return ArjunaNames.Implementation_ObjectStore_ShadowNoFileLockStore();
    }    

    /*
     * Have to return as a ShadowingStore because of
     * inheritence.
     */
    
public static ShadowingStore create ()
    {
	return new ShadowNoFileLockStore("");
    }

    /**
     * message com.arjuna.ats.internal.arjuna.objectstore.ShadowNoFileLockStore_1 [com.arjuna.ats.internal.arjuna.objectstore.ShadowNoFileLockStore_1] - ShadowNoFileLockStore.create caught: {0}
     */
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
		if (shareStatus.intValue() == ObjectStore.OS_SHARED)
		    ss = ObjectStore.OS_SHARED;
	    }
	    catch (Exception e)
	    {
		if (tsLogger.arjLoggerI18N.isWarnEnabled())
		{
		    tsLogger.arjLoggerI18N.warn("com.arjuna.ats.internal.arjuna.objectstore.ShadowNoFileLockStore_1",
						new Object[]{e});
		}
	    }
	}

	return new ShadowNoFileLockStore(location, ss);
    }

public static ShadowingStore create (ObjectName param)
    {
	if (param == null)
	    return null;
	else
	    return new ShadowNoFileLockStore(param);
    }
    
protected ShadowNoFileLockStore (String locationOfStore)
    {
	this(locationOfStore, ObjectStore.OS_UNSHARED);
    }
    
protected ShadowNoFileLockStore (String locationOfStore, int shareStatus)
    {
	super(locationOfStore, shareStatus);

	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PROTECTED,
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

	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PROTECTED,
				     FacilityCode.FAC_OBJECT_STORE, "ShadowNoFileLockStore.ShadowNoFileLockStore( "+shareStatus+" )");
	}
    }

protected ShadowNoFileLockStore (ObjectName objName)
    {
	super(objName);
	
	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PROTECTED,
				     FacilityCode.FAC_OBJECT_STORE, "ShadowNoFileLockStore.ShadowNoFileLockStore( "+objName+" )");
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
