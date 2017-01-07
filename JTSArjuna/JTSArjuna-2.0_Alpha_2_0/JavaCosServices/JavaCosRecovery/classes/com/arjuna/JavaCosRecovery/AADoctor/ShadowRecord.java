/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ShadowRecord.java,v 1.3 1998/07/06 10:49:46 nmcl Exp $
 */

package com.arjuna.JavaCosRecovery.AADoctor;

import com.arjuna.JavaCosRecovery.CrashMan.*;
import com.arjuna.JavaArjunaLite.Atomic.*;
import com.arjuna.JavaArjunaLite.Interface.ObjectStore;
import com.arjuna.JavaGandiva.Common.ClassName;
import com.arjuna.JavaGandiva.Common.Uid;
import java.io.PrintStream;

import com.arjuna.JavaArjunaLite.Common.ObjectStoreException;

/*
 * Class: ShadowRecord
 * Responsible for the recovery of unresolved shadow states.
 */

/*
 * Class: ShadowRecord
 * Responsible for the recovery of uncommitted "shadowed" states.
 * These records are created to identify all of the shadowed states found
 * by crash recovery at reboot time. Some of the states will be resolved
 * by the atomic action recovery that occurs before these records are resolved.
 * However, there will be some states that do not have an associated action
 * object. These are states belonging to actions that crashed after partially
 * completing the prepare phase.
 * Therefore, any states still shadowed after AA recovery is complete should
 * be aborted, which is the purpose of these records.
 */

public class ShadowRecord extends CrashRecRecord
{
    
public ShadowRecord (PrintStream strm, boolean safe, Uid recUid,
		     String recType, ClassName OSType, String OSRoot)
    {
	super(strm, safe);
	
	storeType = null;
	objUid = new Uid(recUid);
	
	status = ShadowStatus.UNRESOLVED_STATE;

	// Initialise storeType
	storeType = OSType;

	// Initialise storeRoot

	if (OSRoot == null)
	    storeRoot = "";
	else
	    storeRoot = OSRoot;

	// Initialise objType
	objType = new String(recType);
    }

public boolean resolve ()
    {
	if (getRecoveryStatus() == RecoveryStatus.COMPLETE)
	    return true;

	outStrm.println("ShadowRecord: Beginning recovery ("+objUid+")");

	// Create the appropriate objectstore type
	ObjectStore store = new ObjectStore(storeType, storeRoot);

	try
	{
	    // Check that this state is still uncommitted
	    if (store.isType(objUid, objType, ObjectStore.OS_UNCOMMITTED))
	    {
		// The state is still shadowed, abort it
		if (store.remove_uncommitted(objUid, objType))
		{
		    status = ShadowStatus.ABORTED_STATE;
		    setRecoveryStatus(RecoveryStatus.COMPLETE);
		}
		else
		{
		    outStrm.println("ShadowRecord: *WARNING* Attempt to remove uncommitted "+"state failed");
		}
	    }
	    else
	    {
		outStrm.println("ShadowRecord: Object is no longer uncommitted, "+
				"no further work required.");
		status = ShadowStatus.IGNORE_STATE;
		setRecoveryStatus(RecoveryStatus.COMPLETE);
	    }
	}
	catch (ObjectStoreException e)
	{
	    outStrm.println("ShadowRecord: ObjectStore error - "+e);
	}
	
	store = null;

	boolean success = ( (getRecoveryStatus() == RecoveryStatus.COMPLETE) ? true : false );

	if (success)
	    outStrm.println("ShadowRecord: Recovery successful ("+objUid+")");
	else
	    outStrm.println("ShadowRecord: Recovery unsuccessful ("+objUid+")");

	checkAlarms();
	
	return success;
    }

public void print (PrintStream strm)
    {
	super.print(strm);
	strm.println("ShadowRecord for uid: "+objUid);
	strm.println("\tType = "+objType);
	strm.println("\tObject store type = "+storeType);
	strm.println("\tRoot = "+storeRoot);
	strm.println("\tStatus = "+status);
    }

public void id (PrintStream strm)
    {
	strm.println("ShadowRecord for uid: "+objUid+", Type = "+objType);
    }

public int type ()
    {
	return CRRecordType.SHADOWRECORD;
    }

public Uid value ()
    {
	return objUid;
    }

public String getType ()
    {
	return objType;
    }

protected void setThreshold (int t)
    {
	shadowThreshold = t;
    }

protected int getThreshold()
    {
	return shadowThreshold;
    }

protected void alert ()
    {
	Mailer m = new Mailer(outStrm);

	m.println("Crash recovery has made "+numberAttempts()
		  +" attempts to resolve the following uncommitted object state:\n");
    
	m.println("\tObject uid  = "+objUid);
	m.println("\tObject type = "+objType);
	m.println("\tObjectStore type = "+storeType);
    
	m.println("\nAttempts are continuing. "+
		  "If this matter is resolved you will be informed.");

	m.sendMessage();
    }

protected void calm ()
    {
	Mailer m = new Mailer(outStrm);

	m.println("Crash recovery has successfully resolved the following "
		  +"object state after "+numberAttempts()+"attempts.\n");

	m.println("\tObject uid  = "+objUid);
	m.println("\tObject type = "+objType);
	m.println("\tObjectStore type = "+storeType);

	m.sendMessage();
	resetAlarms();
    }

private ClassName storeType;
private String storeRoot;
private String objType;
private Uid objUid;
private int status;

private static int shadowThreshold = 2;

}
