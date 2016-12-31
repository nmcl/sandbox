/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: DisposeRecord.java,v 1.2 2000/03/07 09:44:44 nmcl Exp $
 */

/*
 *
 * Dipose Record Class.
 *
 */

package com.arjuna.ArjunaLite.Atomic;

import com.arjuna.ArjunaLite.ArjunaLiteNames;
import com.arjuna.ArjunaLite.Interface.ObjectStore;
import com.arjuna.ArjunaLite.Implementation.ObjectStore.ObjectStoreType;
import com.arjuna.ArjunaCommon.Common.*;
import java.io.PrintStream;

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
    }

public void finalize ()
    {
	store = null;
	typeName = null;

	super.finalize();
    }

public int typeIs ()
    {
	return RecordType.DISPOSE;
    }

public ClassName className ()
    {
	return ArjunaLiteNames.Implementation_AbstractRecord_CadaverRecord_DisposeRecord();
    }
    
public int nestedAbort ()
    {
	return PrepareOutcome.FINISH_OK;
    }

public int nestedCommit ()
    {
	return PrepareOutcome.FINISH_OK;
    }

public int nestedPrepare ()
    {
	if ((store != null) && (objectUid.notEquals(Uid.nullUid())))
	    return PrepareOutcome.PREP_OK;
	else
	    return PrepareOutcome.PREP_NOTOK;
    }

public int topLevelAbort ()
    {
	return PrepareOutcome.FINISH_OK;
    }

    /**
     * At topLevelCommit we remove the state from the object store.
     */

public int topLevelCommit ()
    {
	if ((store != null) && (objectUid.notEquals(Uid.nullUid())))
	{
	    try
	    {
		if (store.remove_committed(objectUid, typeName))
		    return PrepareOutcome.FINISH_OK;
	    }
	    catch (Exception e)
	    {
	    }
	}

	return PrepareOutcome.FINISH_ERROR;
    }

public int topLevelPrepare ()
    {
	if ((store != null) && (objectUid.notEquals(Uid.nullUid())))
	{
	    return PrepareOutcome.PREP_OK;
	}
	else
	    return PrepareOutcome.PREP_NOTOK;
    }

public void print (PrintStream strm)
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
		System.err.println("DisposeRecord::save_state - type of store is unknown");
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
		    System.err.println("DisposeRecord::save_state - failed.");

		    res = false;
		}
	    }
	}
	else
	{
	    System.err.println("DisposeRecord::save_state - no object store defined.");

	    res = false;
	}

	if (res)
	    res = super.save_state(os, ot);

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
		System.err.println("DisposeRecord::restore_state - invalid store type "+objStoreType);
		res = false;
	    }
	}
	catch (IOException e)
	{
	    res = false;
	}

	if (res)
	    res = super.restore_state(os, ot);
    
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

private Uid objectUid;
private String typeName;
private ObjectStore store;

};
