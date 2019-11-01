/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ActivationRecord.java,v 1.1.4.1.6.2 2001/01/12 11:09:49 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Atomic;

import com.arjuna.ArjunaCore.ArjunaCoreNames;
import com.arjuna.ArjunaCommon.Common.*;
import java.io.PrintWriter;

/*
 * Default visibility.
 */

class ActivationRecord extends AbstractRecord
{

    /*
     * This constructor is used to create a new instance of an
     * ActivationRecord.
     */

public ActivationRecord (int st, StateManager sm, BasicAction action)
    {
	super(sm.get_uid(), sm.type(), ObjectType.ANDPERSISTENT);
	
	objectAddr = sm;
	actionHandle = action;
	state = st;

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ABSTRACT_REC, "ActivationRecord::ActivationRecord("+state+", "+sm.get_uid()+")");
	}
    }

public int typeIs ()
    {
	return RecordType.ACTIVATION;
    }

public Object value ()
    {
	return (Object) new Integer(state);
    }

public void setValue (Object v)
    {
	ErrorStream.stream(ErrorStream.WARNING).println("ActivationRecord::set_value() called illegally");
    }

    /**
     * nestedAbort causes the reset_state function of the object to be invoked
     * passing it the saved ObjectStatus.
     */

public int nestedAbort ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ABSTRACT_REC, "ActivationRecord::nestedAbort() for "+order());
	}
	
	int outcome = PrepareOutcome.FINISH_ERROR;
	
	if ((objectAddr != null) && (actionHandle != null))
	    outcome = (objectAddr.forgetAction(actionHandle, false, RecordType.ACTIVATION) ? PrepareOutcome.FINISH_OK : PrepareOutcome.FINISH_ERROR);

	return outcome;
    }

    /**
     * nestedCommit does nothing since the passing of the state up to
     * the parent action is handled by the record list merging system.
     * In fact since nested_prepare returns PREP_READONLY this function should
     * never actually be called.
     */
    
public int nestedCommit ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ABSTRACT_REC, "ActivationRecord::nestedCommit() for "+order());
	}
	
	return PrepareOutcome.FINISH_OK;
    }

public int nestedPrepare ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ABSTRACT_REC, "ActivationRecord::nestedPrepare() for "+order());
	}

	return PrepareOutcome.PREP_READONLY;
    }

    /**
     * topLevelAbort for Activation records is exactly like a nested
     * abort.
     */

public int topLevelAbort ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ABSTRACT_REC, "ActivationRecord::topLevelAbort() for "+order());
	}

	return nestedAbort();		/* i.e., same as nested case */
    }

    /*
     * topLevelCommit has little to do for ActivationRecords other
     * than to ensure the object is forgotten by the object.
     */

public int topLevelCommit ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ABSTRACT_REC, "ActivationRecord::topLevelCommit() for "+order());
	}

	if ((objectAddr != null) && (actionHandle != null))
	{
	    return (objectAddr.forgetAction(actionHandle, true, RecordType.ACTIVATION) ? PrepareOutcome.FINISH_OK : PrepareOutcome.FINISH_ERROR);
	}

	return PrepareOutcome.FINISH_ERROR;
    }

public int topLevelPrepare ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ABSTRACT_REC, "ActivationRecord::topLevelPrepare() for "+order());
	}

	if (objectAddr == null)
	    return PrepareOutcome.PREP_NOTOK;
	else
	    return PrepareOutcome.PREP_OK;
    }

    /**
     * Saving of ActivationRecords is only undertaken during the Prepare
     * phase of the top level 2PC.
     */

public boolean restore_state (InputObjectState os, int v)
    {
	ErrorStream.stream(ErrorStream.WARNING).println("Invocation of ActivationRecord::restore_state for "+
							type()+" inappropriate - ignored for "+order());

	return false;
    }

public boolean save_state (OutputObjectState os, ObjectType v)
    {
	return true;
    }

public void print (PrintWriter strm)
    {
	super.print(strm);
	strm.println("ActivationRecord with state:\n"+state);
    }

public String type ()
    {
	return "/StateManager/AbstractRecord/ActivationRecord";
    }

public void merge (AbstractRecord a)
    {
    }

public void alter (AbstractRecord a)
    {
    }

    /*
     * should_merge and should_replace are invoked by the record list manager
     * to determine if two records should be merged togethor or if the
     * 'newer' should replace the older.
     * shouldAdd determines if the new record should be added in addition
     * to the existing record and is currently only invoked if both of
     * should_merge and should_replace return FALSE
     * Default implementations here always return FALSE - ie new records
     * do not override old
     */

public boolean shouldAdd (AbstractRecord a)
    {
	return false;
    }

public boolean shouldAlter (AbstractRecord a)
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

protected ActivationRecord ()
    {
	super();
	
	objectAddr = null;
	actionHandle = null;
	state = ObjectStatus.PASSIVE;

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PROTECTED,
						 FacilityCode.FAC_ABSTRACT_REC, "ActivationRecord::ActivationRecord()");
	}
    }

private StateManager objectAddr;
private BasicAction  actionHandle;
private int          state;
    
}
