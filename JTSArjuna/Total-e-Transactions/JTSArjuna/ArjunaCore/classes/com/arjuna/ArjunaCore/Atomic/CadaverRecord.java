/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: CadaverRecord.java,v 1.1.4.1.2.1.4.1.10.1 2001/04/06 12:01:12 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Atomic;

import com.arjuna.ArjunaCore.ArjunaCoreNames;
import com.arjuna.ArjunaCore.Interface.ObjectStore;
import com.arjuna.ArjunaCommon.Common.*;
import java.io.PrintWriter;

import com.arjuna.ArjunaCore.Common.ObjectStoreException;

/**
 * Cadaver records are created whenever a persistent object is deleted while
 * still in the scope of an atomic action. This ensures that if the
 * action commits the state of the persistent objects gets properly
 * reflected back in the object store. For objects that are only
 * recoverable such work is unnecessary. Cadaver records replace
 * PersistenceRecords in the record list of an atomic action so they must
 * be merged with such records to enable both commits and aborts to occur.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: CadaverRecord.java,v 1.1.4.1.2.1.4.1.10.1 2001/04/06 12:01:12 nmcl Exp $
 * @since JTS 1.0.
 */

public class CadaverRecord extends PersistenceRecord
{

public CadaverRecord (OutputObjectState os, ObjectStore objStore,
		      StateManager sm)
    {
	super(os, objStore, sm);

	newStateIsValid = ((os != null) ? true : false);
	oldState = null;
	oType = RecordType.NONE_RECORD;
	store = objStore;  // implicit ref count in Java
    
	if (store != null)
	{
	    /*
	     * If the object goes out of scope its object store may
	     * be inaccessable - increase reference count to compensate
	     */

	    /*
	     * Don't need this in Java.
	     */

	    //	    ObjectStore.reference(store);
	}

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ABSTRACT_REC, "CadaverRecord::CadaverRecord("+os+", "+sm.get_uid()+")");
	}
    }

public void finalize ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.DESTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ABSTRACT_REC, "CadaverRecord::finalize() for "+order());
	}

	oldState = null;
	store = null;
	
	super.finalize();
    }

public boolean propagateOnAbort ()
    {
	return true;
    }

public int typeIs ()
    {
	return RecordType.PERSISTENCE;
    }

public ClassName className ()
    {
	return ArjunaCoreNames.Implementation_AbstractRecord_CadaverRecord();
    }

public int nestedAbort ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ABSTRACT_REC, "CadaverRecord::nestedAbort() for "+order());
	}

	if (oldState != null)
	    newStateIsValid = false;
    
	if (oType == RecordType.RECOVERY)
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("Attempted abort operation on deleted object id "+order()+" of type "+getTypeOfObject()+" ignored");
	}
	
	/*
	 * No need to forget the action since this object is
	 * being deleted so it is unlikely to have modified called
	 * on it!
	 */
	
	//	super.forgetAction(false);
    
	return PrepareOutcome.FINISH_OK;
    }

    /**
     * If we have no new state then we cannot commit and must force an
     * abort. Do this by failing the prepare phase
     */

public int nestedPrepare ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ABSTRACT_REC, "CadaverRecord::nestedPrepare() for "+order());
	}

	if (newStateIsValid)
	    return super.nestedPrepare();
	else
	    return PrepareOutcome.PREP_NOTOK;
    }

public int topLevelAbort ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ABSTRACT_REC, "CadaverRecord::topLevelAbort() for "+order());
	}

	newStateIsValid = false;

	if (oType == RecordType.RECOVERY)
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("Attempted abort operation on deleted object id "+order()+" of type "+getTypeOfObject()+" ignored");
	}

	// super.forgetAction(false);
	
	return PrepareOutcome.FINISH_OK;
    }

    /**
     * At topLevelCommit we commit the uncommitted version already saved
     * into object store.
     * Cannot use inherited version since that assumes object is alive
     * instead talk directly to the object store itself
     */

public int topLevelCommit ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ABSTRACT_REC, "CadaverRecord::topLevelCommit() for "+order());
	}

	boolean res = true;
	OutputObjectState oState = super.state;

	if ((oState != null) && (oType == RecordType.PERSISTENCE))
	{
	    if (store == null)
		return PrepareOutcome.FINISH_ERROR;

	    try
	    {
		res = store.commit_state(oState.stateUid(), oState.type());
	    }
	    catch (ObjectStoreException e)
	    {
		res = false;
	    }
	}

	// super.forgetAction(false);
	
	return ((res) ? PrepareOutcome.FINISH_OK : PrepareOutcome.FINISH_ERROR);
    }

    /**
     * At topLevelPrepare write uncommitted version into object store.
     * Cannot use inherited version since that assumes object is alive
     * instead talk directly to the object store itself
     */

public int topLevelPrepare ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ABSTRACT_REC, "CadaverRecord::topLevelPrepare() for "+order());
	}

	int tlpOk = PrepareOutcome.PREP_NOTOK;
	OutputObjectState oState = (newStateIsValid ? super.state : oldState);

	if (oState != null)
	{
	    if (oType == RecordType.PERSISTENCE)
	    {
		if (store == null)
		    return PrepareOutcome.PREP_NOTOK;

		try
		{
		    if (store.write_uncommitted(oState.stateUid(), oState.type(), oState))
		    {
			if (shadowForced())
			    tlpOk = PrepareOutcome.PREP_OK;
		    }
		}
		catch (ObjectStoreException e)
		{
		}
	    }
	    else
		tlpOk = PrepareOutcome.PREP_OK;
	}
    
	return tlpOk;
    }

public void print (PrintWriter strm)
    {
	strm.println("Cadaver for:");
	super.print(strm);
    }

public String type()
    {
	return "/StateManager/AbstractRecord/RecoveryRecord/PersistenceRecord/CadaverRecord";
    }

public boolean doSave ()
    {
	if (oType == RecordType.PERSISTENCE)
	    return true;
	else
	    return false;
    }

    /**
     * Have to return as a AbstractStore because of
     * inheritence.
     */
    
public static AbstractRecord create ()
    {
	return new CadaverRecord();
    }
    
    /**
     * merge takes the information from the incoming PersistenceRecord and
     * uses it to initialise the oldState information. This is required
     * for processing of action aborts since CadaverRecords maintain the
     * final state of an object normally - which is required if the action
     * commits
     */
 
public void merge (AbstractRecord mergewith)
    {
	/*
	 *  Following assumes that value returns a pointer to the 
	 *  old state maintained in the PersistenceRecord (as an ObjectState).
	 *  Here we create a copy of that state allowing the original
	 *  to be deleted
	 */

	oType = mergewith.typeIs();
    
	if (oldState != null)
	{
	    if (newStateIsValid)
	    {
		oldState = null;
	    }
	    else
	    { 
		setValue(oldState);
		newStateIsValid = true;
	    }
	}
    
	oldState = new OutputObjectState((OutputObjectState)(mergewith.value()));
    }

public boolean shouldMerge (AbstractRecord ar)
    {
	return (((order().equals(ar.order())) &&
		 ((ar.typeIs() == RecordType.PERSISTENCE) ||
		  (ar.typeIs() == RecordType.RECOVERY)))
		? true : false);
    }

public boolean shouldReplace (AbstractRecord ar)
    {
	return (((order().equals(ar.order())) &&
		 ((ar.typeIs() == RecordType.PERSISTENCE) ||
		  (ar.typeIs() == RecordType.RECOVERY)))
		? true : false);
    }

protected CadaverRecord ()
    {
	super();

	newStateIsValid = false;
	oldState = null;
	oType = RecordType.NONE_RECORD;
	store = null;

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PROTECTED,
						 FacilityCode.FAC_ABSTRACT_REC, "CadaverRecord::CadaverRecord ()");
	}
    }
    
private boolean           newStateIsValid;
private OutputObjectState oldState;
private int               oType;
private ObjectStore       store;

}
