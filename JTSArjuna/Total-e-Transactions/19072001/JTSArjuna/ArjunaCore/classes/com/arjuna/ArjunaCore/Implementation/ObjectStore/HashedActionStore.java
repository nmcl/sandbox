/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: HashedActionStore.java,v 1.1.2.2.2.2 2001/01/04 12:39:29 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Implementation.ObjectStore;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaCore.ArjunaCoreNames;
import com.arjuna.ArjunaCore.Common.ArjunaCoreEnvironment;
import com.arjuna.ArjunaCore.Atomic.*;
import com.arjuna.ArjunaCommon.Common.ClassName;
import com.arjuna.ArjunaCore.Interface.ObjectStore;
import java.io.*;
import java.io.File;

import com.arjuna.ArjunaCore.Common.ObjectStoreException;
import java.io.IOException;
import java.io.FileNotFoundException;
import java.lang.NumberFormatException;

/**
 * The basic action store implementations store the object states in
 * a separate file within the same directory in the object store, determined
 * by the object's type. However, as the number of file entries within the
 * directory increases, so does the search time for finding a specific file.
 * The HashStore implementation hashes object states over many different
 * sub-directories to attempt to keep the number of files in a given
 * directory low, thus improving performance as the number of object states
 * grows.
 *
 * Currently the hash number is set for both user hashed stores and action
 * hashed stores.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: HashedActionStore.java,v 1.1.2.2.2.2 2001/01/04 12:39:29 nmcl Exp $
 * @since JTS 2.1.
 */

public class HashedActionStore extends HashedStore
{

public int typeIs ()
    {
	return ObjectStoreType.HASHED_ACTION;
    }

public ClassName className ()
    {
	return ArjunaCoreNames.Implementation_ObjectStore_HashedActionStore();
    }

public static ClassName name ()
    {
	return ArjunaCoreNames.Implementation_ObjectStore_HashedActionStore();
    }

    /*
     * Have to return as a ShadowingStore because of
     * inheritence.
     */
    
public static ShadowingStore create ()
    {
	return new HashedActionStore("");
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
		ErrorStream.warning().println("HashedActionStore.create caught: "+e);
	    }
	}

	return new HashedActionStore(location, ss);
    }

    /*
     * Protected constructors and destructor
     */

protected HashedActionStore ()
    {
	this(ObjectStore.OS_UNSHARED);
    }

protected HashedActionStore (int shareStatus)
    {
	super(shareStatus);

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PROTECTED,
					       FacilityCode.FAC_OBJECT_STORE, "HashedStore.HashedActionStore()");
	}
    }

protected HashedActionStore (String locationOfStore)
    {
	this(locationOfStore, ObjectStore.OS_UNSHARED);
    }
    
protected HashedActionStore (String locationOfStore, int shareStatus)
    {
	super(shareStatus);

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PROTECTED,
					       FacilityCode.FAC_OBJECT_STORE, "HashedStore.HashedActionStore("+locationOfStore+")");
	}

	try
	{
	    setupStore(locationOfStore);
	}
	catch (ObjectStoreException e)
	{
	    ErrorStream.stream(ErrorStream.WARNING).println(e.getMessage());

	    throw new com.arjuna.ArjunaCommon.Common.FatalError(e.toString());
	}
    }
    
}

