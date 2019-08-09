/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: DisposeRecord.java,v 1.1.4.1.2.2.4.1.4.1.2.1 2001/02/19 10:57:54 nmcl Exp $
 */

/*
 *
 * Dipose Record Class.
 *
 */

package com.arjuna.ArjunaCore.Atomic;

import com.arjuna.ArjunaCore.ArjunaCoreNames;
import com.arjuna.ArjunaCore.Interface.ObjectStore;
import com.arjuna.ArjunaCore.Implementation.ObjectStore.ObjectStoreType;
import com.arjuna.ArjunaCommon.Common.*;
import java.io.PrintWriter;

import java.io.IOException;

class DisposeRecord extends CadaverRecord
{

public DisposeRecord (ObjectStore objStore, StateManager sm)
    {
	super(null, objStore, sm);

	store = objStore;

	if (sm != null)
	{
	    objectUid = sm.get_uid();
	    typeName = sm.type();
	}
	else
	{
	    objectUid = Uid.nullUid();
	    typeName = null;
	}

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ABSTRACT_REC, "DisposeRecord::DisposeRecord("+objStore+", "+objectUid+")");
	}
    }

public void finalize ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.DESTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ABSTRACT_REC, "DisposeRecord.finalize for "+order());
	}

	store = null;
	typeName = null;

	super.finalize();
    }

public boolean propagateOnAbort ()
    {
	return false;
    }

public int typeIs ()
    {
	return RecordType.DISPOSE;
    }

public ClassName className ()
    {
	return ArjunaCoreNames.Implementation_AbstractRecord_CadaverRecord_DisposeRecord();
    }
    
public int nestedAbort ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ABSTRACT_REC, "DisposeRecord::nestedAbort() for "+order());
	}

	return PrepareOutcome.FINISH_OK;
    }

public int nestedCommit ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ABSTRACT_REC, "DisposeRecord::nestedCommit() for "+order());
	}

	return PrepareOutcome.FINISH_OK;
    }

public int nestedPrepare ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ABSTRACT_REC, "DisposeRecord::nestedPrepare() for "+order());
	}

	if ((store != null) && (objectUid.notEquals(Uid.nullUid())))
	    return PrepareOutcome.PREP_OK;
	else
	    return PrepareOutcome.PREP_NOTOK;
    }

public int topLevelAbort ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ABSTRACT_REC, "DisposeRecord::topLevelAbort() for "+order());
	}

	return PrepareOutcome.FINISH_OK;
    }

    /**
     * At topLevelCommit we remove the state from the object store.
     */

public int topLevelCommit ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ABSTRACT_REC, "DisposeRecord::topLevelCommit() for "+order());
	}

	if ((store != null) && (objectUid.notEquals(Uid.nullUid())))
	{
	    try
	    {
		if (store.remove_committed(objectUid, typeName))
		{
		    super.objectAddr.destroyed();
		    
		    return PrepareOutcome.FINISH_OK;
		}
	    }
	    catch (Exception e)
	    {
	    }
	}

	return PrepareOutcome.FINISH_ERROR;
    }

public int topLevelPrepare ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ABSTRACT_REC, "DisposeRecord::topLevelPrepare() for "+order());
	}

	if ((store != null) && (objectUid.notEquals(Uid.nullUid())))
	{
	    return PrepareOutcome.PREP_OK;
	}
	else
	    return PrepareOutcome.PREP_NOTOK;
    }

public void print (PrintWriter strm)
    {
	strm.println("Dispose for:");
	super.print(strm);
    }

public boolean doSave ()
    {
	return true;
    }

public boolean save_state (OutputObjectState os, int ot)
    {
	boolean res = true;
	
	if ((store != null) && (objectUid.notEquals(Uid.nullUid())))
	{
	    if (!ObjectStoreType.valid(store.typeIs()))
	    {
		ErrorStream.stream(ErrorStream.WARNING).println("DisposeRecord::save_state - type of store is unknown");

		res = false;
	    }
	    else
	    {
		try
		{
		    os.packInt(store.typeIs());
		    store.pack(os);

		    objectUid.pack(os);
		    os.packString(typeName);
		}
		catch (IOException e)
		{
		    ErrorStream.stream(ErrorStream.WARNING).println("DisposeRecord::save_state - failed.");

		    res = false;
		}
	    }
	}
	else
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("DisposeRecord::save_state - no object store defined.");

	    res = false;
	}

	return res;
    }

public boolean restore_state (InputObjectState os, int ot)
    {
	boolean res = false;
	int objStoreType = 0;

	try
	{
	    objStoreType = os.unpackInt();

	    if (ObjectStoreType.valid(objStoreType))
	    {
		store = null;

		store = new ObjectStore(ObjectStoreType.typeToClassName(objStoreType));
		store.unpack(os);
	    
		objectUid.unpack(os);
		typeName = os.unpackString();
	    }
	    else
	    {
		ErrorStream.stream(ErrorStream.WARNING).println("DisposeRecord::restore_state - invalid store type "+objStoreType);
		res = false;
	    }
	}
	catch (IOException e)
	{
	    res = false;
	}

	return res;
    }
    
public String type ()
    {
	return "/StateManager/AbstractRecord/RecoveryRecord/PersistenceRecord/CadaverRecord/DisposeRecord";
    }

public boolean shouldAdd (AbstractRecord a)
    {
	return false;
    }

public boolean shouldMerge (AbstractRecord a)
    {
	return false;
    }

public boolean shouldReplace (AbstractRecord a)
    {
	return false;
    }

public boolean shouldAlter (AbstractRecord a)
    {
	return false;
    }

private Uid         objectUid;
private String      typeName;
private ObjectStore store;

};
