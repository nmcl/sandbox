/*
 * Copyright (C) 1996, 1997, 1998, 1999,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: PersistenceRecord.java,v 1.11 1998/12/11 14:24:23 nmcl Exp $
 */

package com.arjuna.JavaArjuna.ClassLib;

import com.arjuna.JavaArjuna.ObjectStore.ObjectStore;
import com.arjuna.JavaArjuna.Common.*;
import java.io.PrintStream;

import com.arjuna.JavaArjuna.Common.ObjectStoreException;
import java.io.IOException;

public class PersistenceRecord extends RecoveryRecord
{
    
    /*
     * This constructor is used to create a new instance of
     * PersistenceRecord.
     */

public PersistenceRecord (OutputObjectState os, ObjectStore objStore,
			  StateManager sm)
    {
	super(os, sm);
	
	shadowMade = false;
	store = objStore;
	topLevelState = null;
    }

public void finalize ()
    {
	store = null;
	topLevelState = null;
    }

    /*
     * Redefintions of public virtual functions inherited from RecoveryRecord
     */

public int typeIs ()
    {
	return RecordType.PERSISTENCE;
    }

    /*
     * topLevelAbort may have to remove the persistent state that was written
     * into the object store during the processing of topLevelPrepare.
     * It then does the standard abort processing.
     */

public int topLevelAbort ()
    {
	if (shadowMade)
	{
	    try
	    {
		if ((topLevelState == null) ||
		    (!store.remove_uncommitted(topLevelState.stateUid(), topLevelState.type())))
		{
		    return PrepareOutcome.FINISH_ERROR;
		}
	    }
	    catch (ObjectStoreException e)
	    {
		return PrepareOutcome.FINISH_ERROR;
	    }
	}
    
	return nestedAbort();
    }

    /*
     * commit the state saved during the prepare phase.
     */

public int topLevelCommit ()
    {
	boolean result = false;

	if (store != null)
	{
	    try
	    {
		if (shadowMade)
		    result = store.commit_state(order(), super.getTypeOfObject());
		else
		    if (topLevelState != null)
			result = store.write_committed(order(), super.getTypeOfObject(), topLevelState);
	    }
	    catch (ObjectStoreException e)
	    {
		System.err.println("PersistenceRecord::topLevelCommit - caught exception: "+e);
		
		result = false;
	    }
	}

	if (!result)
	{
	    System.err.println("PersistenceRecord::topLevelCommit - commit_state error");
	}

	super.forgetAction(true);
	
	return ((result) ? PrepareOutcome.FINISH_OK : PrepareOutcome.FINISH_ERROR);
    }

    /*
     * topLevelPrepare attempts to save the object.
     * It will either do this in the action intention list or directly
     * in the object store by using the 'deactivate' function of the object
     * depending upon the size of the state.
     * To ensure that objects are correctly hidden while they are in an
     * uncommitted state if we use the abbreviated protocol then we write an
     * EMPTY object state as the shadow state - THIS MUST NOT BE COMMITTED.
     * Instead we write_committed the one saved in the intention list.
     * If the store cannot cope with being given an empty state we revert to
     * the old protocol
     */

public int topLevelPrepare ()
    {
	int result = PrepareOutcome.PREP_NOTOK;
	StateManager sm = super.objectAddr;
    
	if ((sm != null) && (store != null))
	{
	    topLevelState = new OutputObjectState(sm.get_uid(), sm.type());
	    
	    if (!store.fullCommitNeeded() &&
		(sm.save_state(topLevelState, ObjectType.ANDPERSISTENT)) &&
		(topLevelState.size() <= PersistenceRecord.MAX_OBJECT_SIZE))
	    {
		OutputObjectState dummy = new OutputObjectState(Uid.nullUid(), null);

		/*
		 * Write an empty shadow state to the store to indicate
		 * one exists, and to prevent bogus activation in the case
		 * where crash recovery hasn't run yet.
		 */

		try
		{
		    store.write_uncommitted(sm.get_uid(), sm.type(), dummy);
		    result = PrepareOutcome.PREP_OK;
		}
		catch (ObjectStoreException e)
		{
		}
		
		dummy = null;
	    }
	    else
	    {
		if (sm.deactivate(store.getStoreName(), false))
		{
 		    shadowMade = true;
		    result = PrepareOutcome.PREP_OK;
		}
		else
		    System.err.println("PersistenceRecord deactivate error");
	    }
	}

	return result;
    }

    /*
     * topLevelCleanup must leave the persistent state that was written
     * into the object store during the processing of topLevelPrepare intact.
     * Crash recovery will take care of its resolution
     */

public int topLevelCleanup ()
    {
	return PrepareOutcome.FINISH_OK;
    }

public boolean doSave ()
    {
	return true;
    }
    
public boolean restore_state (InputObjectState os, int ot)
    {
	boolean res = false;
	String objStoreType = null;

	try
	{
	    objStoreType = os.unpackString();

	    if (objStoreType != null)
	    {
		/* discard old store before creating new */

		/*
		 * https://github.com/nmcl/sandbox/issues/66
		store = new ObjectStore(new ClassName(objStoreType));
		*/

		store.unpack(os);
		shadowMade = os.unpackBoolean();

		topLevelState = null;

		if (!shadowMade)
		{
		    topLevelState = new OutputObjectState(os);
		    res = topLevelState.valid();
		}
		else
		    res = true;

		objStoreType = null;

		return (res && super.restore_state(os, ot));
	    }
	}
	catch (IOException e)
	{
	    System.err.println("PersistenceRecord::restore_state: Failed to unpack object store type");
	}
    
	return res;
    }

public boolean save_state (OutputObjectState os, int ot)
    {
	boolean res = true;

	if (store != null)
	{
	    /*
	     * https://github.com/nmcl/sandbox/issues/66
	    if ((store.className() == null) || (store.className().equals("")))
	    {
		System.err.println("Type of store is unknown");
	    }
	    else
	    {
		try
		{
		    os.packString(store.className().stringForm());
		    store.pack(os);
		    os.packBoolean(shadowMade);
		    
		    if (!shadowMade)
		    {
			res = (topLevelState != null);

			if (res)
			    topLevelState.packInto(os);
			else
			{
			    System.err.println("Packing top level state failed\n");
			}
		    }
		}
		catch (IOException e)
		{
		    res = false;
		    
		    System.err.println("PersistenceRecord save_state failed.");
		}
	    }
	    */
	}
	else
	{
	    System.err.println("No object store defined for object");

	    try
	    {
		os.packString(null);
	    }
	    catch (IOException e)
	    {
		res = false;
	    }
	}
    
	return res && super.save_state(os, ot);
    }

public void print (PrintStream strm)
    {
	super.print(strm);	/* bypass RecoveryRecord */

	strm.println("PersistenceRecord with state:\n"+super.state);
    }

public String type ()
    {
	return "/StateManager/AbstractRecord/RecoveryRecord/PersistenceRecord";
    }

public static AbstractRecord create ()
    {
	return new PersistenceRecord();
    }

    /*
     * Creates a 'blank' persistence record. This is used during crash recovery
     * when recreating the prepared list of a server atomic action.
     */
    
protected PersistenceRecord ()
    {
	super();
	shadowMade = false;
	store = null;
	topLevelState = null;
    }

    /*
     * Cadaver records force write shadows.
     * This operation supresses to abbreviated commit
     * This should never return false
     */

protected boolean shadowForced ()
    {
	if (topLevelState == null)
	{
	    shadowMade = true;
	    return true;
	}

	/* I've already done the abbreviated protocol so its too late */
    
	return false;
    }

public static final int MAX_OBJECT_SIZE = 4096;  // block size
    
private boolean shadowMade;
private ObjectStore store;
private OutputObjectState topLevelState;
    
}
