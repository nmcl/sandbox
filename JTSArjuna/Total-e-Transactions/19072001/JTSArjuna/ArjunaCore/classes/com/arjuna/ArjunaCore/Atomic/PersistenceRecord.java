/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: PersistenceRecord.java,v 1.1.4.1.2.2.2.1.2.2.20.1.36.1 2001/08/08 14:18:52 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Atomic;

import com.arjuna.ArjunaCore.ArjunaCoreNames;
import com.arjuna.ArjunaCore.Common.ArjunaCoreEnvironment;
import com.arjuna.ArjunaCore.Interface.ObjectStore;
import com.arjuna.ArjunaCore.Implementation.ObjectStore.ObjectStoreType;
import com.arjuna.ArjunaCommon.Common.*;
import java.io.PrintWriter;

import com.arjuna.ArjunaCore.Common.ObjectStoreException;
import java.io.IOException;

/**
 * A PersistenceRecord is created whenever a persistent object is
 * created/read/modified within the scope of a transaction. It is
 * responsible for ensuring that state changes are committed or rolled back
 * on behalf of the object depending upon the outcome of the transaction.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: PersistenceRecord.java,v 1.1.4.1.2.2.2.1.2.2.20.1.36.1 2001/08/08 14:18:52 nmcl Exp $
 * @since JTS 1.0.
 */

public class PersistenceRecord extends RecoveryRecord
{
    
    /**
     * This constructor is used to create a new instance of
     * PersistenceRecord.
     */

public PersistenceRecord (OutputObjectState os, ObjectStore objStore,
			  StateManager sm)
    {
	super(os, sm);

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ABSTRACT_REC, "PersistenceRecord::PersistenceRecord("
						 +os+", "+sm.get_uid()+")");
	}
	
	shadowMade = false;
	store = objStore;
	topLevelState = null;
    }

public void finalize ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.DESTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ABSTRACT_REC, "PersistenceRecord.finalize() for "+order());
	}

	store = null;
	topLevelState = null;

	super.finalize();
    }

    /**
     * Redefintions of abstract functions inherited from RecoveryRecord.
     */

public int typeIs ()
    {
	return RecordType.PERSISTENCE;
    }

public ClassName className ()
    {
	return ArjunaCoreNames.Implementation_AbstractRecord_PersistenceRecord();
    }
    
    /**
     * topLevelAbort may have to remove the persistent state that was written
     * into the object store during the processing of topLevelPrepare.
     * It then does the standard abort processing.
     */

public int topLevelAbort ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ABSTRACT_REC, "PersistenceRecord::topLevelAbort() for "+order());
	}

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

    /**
     * commit the state saved during the prepare phase.
     */

public int topLevelCommit ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ABSTRACT_REC, "PersistenceRecord::topLevelCommit() for "+order());

	    DebugController.controller().print(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_ABSTRACT_REC, "PersistenceRecord::topLevelCommit() : About to commit "+" state, uid = "+order()+", ObjType = "+getTypeOfObject());

	    if (store != null)
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						     FacilityCode.FAC_ABSTRACT_REC, ", store = "+store+"("+store.typeIs()+")");
	    else
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						     FacilityCode.FAC_ABSTRACT_REC, "");
	}

	boolean result = false;

	if (store != null)
	{
	    try
	    {
		if (shadowMade)
		{
		    result = store.commit_state(order(), super.getTypeOfObject());

		    if (!result)
			ErrorStream.warning().println("PersistenceRecord::topLevelCommit - commit_state call failed for "+order());
		}
		else
		{
		    if (topLevelState != null)
			result = store.write_committed(order(), super.getTypeOfObject(), topLevelState);
		    else
			ErrorStream.warning().println("PersistenceRecord::topLevelCommit - no state to commit!");
		}
	    }
	    catch (ObjectStoreException e)
	    {
		ErrorStream.stream(ErrorStream.WARNING).println("PersistenceRecord::topLevelCommit - caught exception: "+e);
		
		result = false;
	    }
	}
	else
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("PersistenceRecord::topLevelCommit - no object store specified!");
	}

	if (!result)
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("PersistenceRecord::topLevelCommit - commit_state error");
	}

	super.forgetAction(true);
	
	return ((result) ? PrepareOutcome.FINISH_OK : PrepareOutcome.FINISH_ERROR);
    }

    /**
     * topLevelPrepare attempts to save the object.
     * It will either do this in the action intention list or directly
     * in the object store by using the 'deactivate' function of the object
     * depending upon the size of the state.
     * To ensure that objects are correctly hidden while they are in an
     * uncommitted state if we use the abbreviated protocol then we write an
     * EMPTY object state as the shadow state - THIS MUST NOT BE COMMITTED.
     * Instead we write_committed the one saved in the intention list.
     * If the store cannot cope with being given an empty state we revert to
     * the old protocol.
     */

public int topLevelPrepare ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ABSTRACT_REC, "PersistenceRecord::topLevelPrepare() for "+order());
	}

	int result = PrepareOutcome.PREP_NOTOK;
	StateManager sm = super.objectAddr;
    
	if ((sm != null) && (store != null))
	{
	    topLevelState = new OutputObjectState(sm.get_uid(), sm.type());
	    
	    if (!store.fullCommitNeeded() &&
		(sm.save_state(topLevelState, ObjectType.ANDPERSISTENT)) &&
		(topLevelState.size() <= PersistenceRecord.MAX_OBJECT_SIZE))
	    {
		if (PersistenceRecord.classicPrepare)
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
			ErrorStream.warning().println("PersistenceRecord.topLevelPrepare caught: "+e);
		    }
		
		    dummy = null;
		}
		else
		    result = PrepareOutcome.PREP_OK;
	    }
	    else
	    {
		if (sm.deactivate(store.getStoreName(), false))
		{
 		    shadowMade = true;
		    result = PrepareOutcome.PREP_OK;
		}
		else
		    ErrorStream.stream(ErrorStream.WARNING).println("PersistenceRecord deactivate error");
	    }
	}
	else
	{
	    ErrorStream.warning().println("PersistenceRecord.topLevelPrepare - setup error!");
	}

	return result;
    }

    /**
     * topLevelCleanup must leave the persistent state that was written
     * in the object store during the processing of topLevelPrepare intact.
     * Crash recovery will take care of its resolution
     */

public int topLevelCleanup ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ABSTRACT_REC, "PersistenceRecord::topLevelCleanup() for "+order());
	}

	return PrepareOutcome.FINISH_OK;
    }

    /**
     * @return <code>true</code>
     */

public boolean doSave ()
    {
	return true;
    }
    
public boolean restore_state (InputObjectState os, int ot)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ABSTRACT_REC, "PersistenceRecord::restore_state() for "+order());
	}

	boolean res = false;
	int objStoreType = 0;

	try
	{
	    objStoreType = os.unpackInt();

	    if (DebugController.enabled())
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						     FacilityCode.FAC_ABSTRACT_REC, "PersistenceRecord::restore_state: "
						     +"Just unpacked object store type = "+objStoreType);
	    }

	    if (ObjectStoreType.valid(objStoreType))
	    {
		/* discard old store before creating new */

		store = new ObjectStore(ObjectStoreType.typeToClassName(objStoreType));

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

		return (res && super.restore_state(os, ot));
	    }
	}
	catch (IOException e)
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("PersistenceRecord::restore_state: Failed to unpack object store type");
	}
    
	return res;
    }

public boolean save_state (OutputObjectState os, int ot)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ABSTRACT_REC, "PersistenceRecord::save_state() for "+order());
	}

	boolean res = true;

	if (store != null)
	{
	    if (!ObjectStoreType.valid(store.typeIs()))
	    {
		ErrorStream.stream(ErrorStream.WARNING).println("PersistenceRecord::save_state - type of store is unknown");
		res = false;
	    }
	    else
	    {
		try
		{
		    os.packInt(store.typeIs());

		    if (DebugController.enabled())
		    {
			DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
							     FacilityCode.FAC_ABSTRACT_REC, "PersistenceRecord::save_state: "
							     +"Packed object store type = "+store.typeIs());
		    }

		    store.pack(os);

		    if (DebugController.enabled())
		    {
			DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
							     FacilityCode.FAC_ABSTRACT_REC, "PersistenceRecord::save_state: "
							     +"Packed object store root");
		    }

		    os.packBoolean(shadowMade);
		    
		    if (!shadowMade)
		    {
			res = (topLevelState != null);

			if (res)
			    topLevelState.packInto(os);
			else
			{
			    ErrorStream.stream(ErrorStream.WARNING).println("PersistenceRecord::save_state - packing top level state failed\n");
			}
		    }
		}
		catch (IOException e)
		{
		    res = false;
		    
		    ErrorStream.stream(ErrorStream.WARNING).println("PersistenceRecord::save_state - failed.");
		}
	    }
	}
	else
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("PersistenceRecord::save_state - no object store defined for object");

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

public void print (PrintWriter strm)
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

    /**
     * Creates a 'blank' persistence record. This is used during crash recovery
     * when recreating the prepared list of a server atomic action.
     */
    
protected PersistenceRecord ()
    {
	super();

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PROTECTED,
						 FacilityCode.FAC_ABSTRACT_REC, "PersistenceRecord::PersistenceRecord()"
						 +"- crash recovery constructor");
	}

	shadowMade = false;
	store = null;
	topLevelState = null;
    }

    /**
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
    
private boolean           shadowMade;
private ObjectStore       store;
private OutputObjectState topLevelState;
    
private static boolean classicPrepare = false;
    
    static
    {
	String cp = PropertyManager.getProperty(ArjunaCoreEnvironment.CLASSIC_PREPARE);
	
	if (cp != null)
	{
	    if (cp.equals("YES"))
		classicPrepare = true;
	}
    }
    
}
