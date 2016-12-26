/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ActionStore.java,v 1.1.4.1.2.2.2.3.2.1 2000/12/21 11:21:09 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Implementation.ObjectStore;

import com.arjuna.ArjunaCore.ArjunaCoreNames;
import com.arjuna.ArjunaCore.Common.ArjunaCoreEnvironment;
import com.arjuna.ArjunaCore.Atomic.*;
import com.arjuna.ArjunaCore.Interface.ObjectStore;
import com.arjuna.ArjunaCommon.Common.*;
import java.io.File;

import com.arjuna.ArjunaCore.Common.ObjectStoreException;
import java.io.IOException;

/**
 * The basic transaction log implementation. Uses the no file-level
 * locking implementation of the file system store since only a single user
 * (the coordinator) can ever be manipulating the action's state.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ActionStore.java,v 1.1.4.1.2.2.2.3.2.1 2000/12/21 11:21:09 nmcl Exp $
 * @since JTS 1.0.
 */

public class ActionStore extends ShadowNoFileLockStore
{

public int typeIs ()
    {
	return ObjectStoreType.ACTION;
    }
    
    /**
     * @return current state of object. Assumes that genPathName allocates
     * enough extra space to allow extra chars to be added.
     * Action stores only store committed objects
     */

public int currentState (Uid objUid, String tName) throws ObjectStoreException
    {
	int theState = ObjectStore.OS_UNKNOWN;
    
	if (storeValid())
	{
	    String path = genPathName(objUid, tName, ObjectStore.OS_ORIGINAL);

	    if (exists(path))
		theState = ObjectStore.OS_COMMITTED;

	    path = null;
	}

	if (DebugController.enabled())
	{
	    synchronized (DebugController.controller())
		{
		    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						       FacilityCode.FAC_OBJECT_STORE, "ActionStore.currentState("+objUid+", "+tName+") - returning ");

		    if (DebugController.controller().debugAllowed(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
								  FacilityCode.FAC_OBJECT_STORE))
		    {
			ObjectStore.printStateStatus(DebugController.controller().getStream(), theState);
		    }

		    if (DebugController.controller().debugAllowed(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
								  FacilityCode.FAC_OBJECT_STORE))
		    {
			DebugController.controller().println();
		    }
		}
	}

	return theState;
    }

    /**
     * Commit a previous write_state operation which
     * was made with the SHADOW StateType argument. This is achieved by
     * renaming the shadow and removing the hidden version.
     */

public synchronized boolean commit_state (Uid objUid,
					  String tName) throws ObjectStoreException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OBJECT_STORE, "ActionStore.commit_state("+objUid+", "+tName+")");
	}

	boolean result = false;

	/* Bail out if the object store is not set up */

	if (!storeValid())
	    return false;

	if (currentState(objUid, tName) == ObjectStore.OS_COMMITTED)
	    result = true;
    
	return result;
    }

public boolean hide_state (Uid u, String tn) throws ObjectStoreException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OBJECT_STORE, "ActionStore.hide_state("+u+", "+tn+")");
	}

	return false;
    }

public boolean reveal_state (Uid u, String tn) throws ObjectStoreException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OBJECT_STORE, "ActionStore.reveal_state("+u+", "+tn+")");
	}

	return false;
    }

public InputObjectState read_committed (Uid storeUid, String tName) throws ObjectStoreException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OBJECT_STORE, "ActionStore.read_committed("+storeUid+", "+tName+")");
	}

	return super.read_committed(storeUid, tName);
    }

public InputObjectState read_uncommitted (Uid u, String tn) throws ObjectStoreException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OBJECT_STORE, "ActionStore.read_uncommitted("+u+", "+tn+")");
	}

	return null;
    }

public boolean remove_committed (Uid storeUid, String tName) throws ObjectStoreException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OBJECT_STORE, "ActionStore.remove_committed("+storeUid+", "+tName+")");
	}

	return super.remove_committed(storeUid, tName);
    }

public boolean remove_uncommitted (Uid u, String tn) throws ObjectStoreException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OBJECT_STORE, "ActionStore.remove_uncommitted("+u+", "+tn+")");
	}

	return false;
    }

public boolean write_committed (Uid storeUid, String tName, OutputObjectState state) throws ObjectStoreException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OBJECT_STORE, "ActionStore.write_committed("+storeUid+", "+tName+")");
	}

	return super.write_committed(storeUid, tName, state);
    }

public boolean write_uncommitted (Uid u, String tn, OutputObjectState s) throws ObjectStoreException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OBJECT_STORE, "ActionStore.write_uncommitted("+u+", "+tn+", "+s+")");
	}

	return false;
    }

public ClassName className ()
    {
	return ArjunaCoreNames.Implementation_ObjectStore_ActionStore();
    }

public static ClassName name ()
    {
	return ArjunaCoreNames.Implementation_ObjectStore_ActionStore();
    }    

    /**
     * Have to return as a ShadowingStore because of
     * inheritence.
     */
    
public static ShadowingStore create ()
    {
	return new ActionStore("");
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
		ErrorStream.warning().println("ActionStore.create caught: "+e);
	    }
	}
	
	return new ActionStore(location, ss);
    }
    
protected ActionStore (String locationOfStore)
    {
	this(locationOfStore, ObjectStore.OS_UNSHARED);
    }

protected ActionStore (String locationOfStore, int shareStatus)
    {
	super(shareStatus);

	try
	{
	    setupStore(locationOfStore);
	}
	catch (ObjectStoreException e)
	{
	    ErrorStream.warning().println(e.getMessage());

	    super.makeInvalid();
	    
	    throw new com.arjuna.ArjunaCommon.Common.FatalError(e.toString());
	}
    }

protected ActionStore ()
    {
	this(ObjectStore.OS_UNSHARED);
    }

protected ActionStore (int shareStatus)
    {
	super(shareStatus);
    }
    
protected synchronized boolean setupStore (String location) throws ObjectStoreException
    {
	if (!checkSync)
	{
	    String syncOpt = PropertyManager.getProperty(ArjunaCoreEnvironment.TRANSACTION_SYNC);

	    if (syncOpt != null)
	    {
		if (syncOpt.compareTo("OFF") == 0)
		    syncOff();
		else
		    syncOn();
	    }
	    else
		syncOn();

	    checkSync = true;
	}

	return super.setupStore(location);
    }
    
private static boolean checkSync = false;
    
}
