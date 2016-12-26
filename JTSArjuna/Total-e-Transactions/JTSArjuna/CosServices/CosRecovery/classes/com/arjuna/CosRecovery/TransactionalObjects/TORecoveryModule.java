/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: TORecoveryModule.java,v 1.1.2.3.2.1.4.1.30.1 2001/07/19 14:00:53 nmcl Exp $
 */

package com.arjuna.CosRecovery.TransactionalObjects;

import org.omg.CosTransactions.*;

import java.util.*;
import java.io.PrintWriter;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaCore.Interface.ObjectStore;
import com.arjuna.ArjunaCore.*;
import com.arjuna.ArjunaCore.Common.*;
import com.arjuna.ArjunaCore.Atomic.*;
import com.arjuna.CosTransactions.*;

import com.arjuna.CosRecovery.RecoveryModule;

import java.io.IOException;

/**
 * This class is a plug-in module for the recovery manager.  This
 * class is responsible for the recovery of Transactional Objects (aka
 * AIT objects), i.e., objects that derive from LockManager.
 */

public class TORecoveryModule implements RecoveryModule
{
    public TORecoveryModule ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_CRASH_RECOVERY,
					       "TORecoveryModule created");
	}

	// where are TO's stored (this will need changing if users know how to change the ostore)
	if (_objectStore == null)
	    _objectStore = new ObjectStore(ObjectStore.OS_SHARED);
    }

    public void periodicWorkFirstPass ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_RECOVERY_NORMAL,
						 "TORecoveryModule - first pass");
	}

	// Build a hashtable of uncommitted transactional objects
	_uncommittedTOTable = new Hashtable();

	try
	{
	    InputObjectState types = new InputObjectState();

	    // find all the types of transactional object (in this ObjectStore)
	    if (_objectStore.allTypes(types))
	    {
		String theName = null;

		try
		{
		    boolean endOfList = false;

		    while (!endOfList)
		    {
			// extract a type
			theName = types.unpackString();

			if (theName.compareTo("") == 0)
			    endOfList = true;
			else
			{
			    InputObjectState uids = new InputObjectState();
			    
			    // find the uids of anything with an uncommitted entry in the object store
			    if (_objectStore.allObjUids(theName, uids, ObjectStore.OS_UNCOMMITTED))
			    {
				Uid theUid = new Uid(Uid.nullUid());

				try
				{
				    boolean endOfUids = false;

				    while (!endOfUids)
				    {
					// extract a uid
					theUid.unpack(uids);

					if (theUid.equals(Uid.nullUid()))
					    endOfUids = true;
					else
					{
					    String newTypeString = new String(theName);
					    Uid newUid = new Uid(theUid);
					    _uncommittedTOTable.put(newUid, newTypeString);
					    if (DebugController.enabled())
					    {
						DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
							FacilityCode.FAC_CRASH_RECOVERY,
							"TO currently uncommitted " + newUid + " is a " + newTypeString);
					    }
					}
				    }
				}
				catch (Exception e)
				{
				    // end of uids!
				}
			    }
			}
		    }
		}
		catch (IOException ex)
		{
		    // nothing there.
		}
		catch (Exception e)
		{
		    ErrorStream.warning().println("TORecoveryModule: "
						  +"searching for TOs: "+e);
		}
	    }
	}
	catch (Exception e)
	{
	    ErrorStream.warning().println("TORecoveryModule: "
					  +"searching for TOs: "+e);
	}

    }

    public void periodicWorkSecondPass ()
    {
	if (DebugController.enabled()) 
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_RECOVERY_NORMAL,
						 "TORecoveryModule - second pass");
	}

	Enumeration uncommittedObjects = _uncommittedTOTable.keys();

	while (uncommittedObjects.hasMoreElements())
	{
	    Uid objUid = (Uid) uncommittedObjects.nextElement();
	    String objType = (String) _uncommittedTOTable.get(objUid);

	    try
	    {
		if (_objectStore.currentState(objUid, objType) == ObjectStore.OS_UNCOMMITTED)
		{
		    recoverObject(objUid, objType);
		}
		else
		{
		    if (DebugController.enabled())
		    {
			DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
							     FacilityCode.FAC_CRASH_RECOVERY,
							     "TORecoveryModule.periodicWork(): "
							     +"Object ("+objUid+", "+objType+") "
							     +"is no longer uncommitted.");
		    }
		}
	    }
	    catch (ObjectStoreException ose)
	    {
		if (DebugController.enabled())
		{
		    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
							 FacilityCode.FAC_CRASH_RECOVERY,
							 "TORecoveryModule.periodicWork(): "
							 +"Object ("+objUid+", "+objType+") "
							 +"no longer exists.");
		}
	    }
	}
    }

    /**
     * Set-up routine
     */
    protected void initialise ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PROTECTED,
						 FacilityCode.FAC_CRASH_RECOVERY,
						 "TORecoveryModule.initialise()");
	}
    }

    private void recoverObject(Uid objUid, String objType)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PRIVATE,
						 FacilityCode.FAC_RECOVERY_NORMAL,
						 "TORecoveryModule.recoverObject("
						 +objUid+", "+objType+")");
	}

	// get a shell of the TO and find out which transaction it was that got it uncommitted
	 
	RecoveredTransactionalObject recoveredTO = 
		new RecoveredTransactionalObject(objUid, objType, _objectStore);
	
	// tell it to replayPhase2, in whatever way it does
	// (in fact it won't do anything unless it determines the
	//  transaction rolled back)
	recoveredTO.replayPhase2();

    }

    private static ObjectStore _objectStore = null;
    private Hashtable	   _uncommittedTOTable;

};
