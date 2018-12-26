/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ActivationRecord.java,v 1.9 1998/07/06 13:26:13 nmcl Exp $
 */

package com.arjuna.JavaArjuna.ClassLib;

import com.arjuna.JavaArjuna.Common.*;
import java.io.PrintStream;

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
	state = st;
	actionHandle = action;
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
	System.err.println("ActivationRecord::set_value() called illegally");
    }

    /*
     * nestedAbort causes the reset_state function of the object to be invoked
     * passing it the saved ObjectStatus
     */

public int nestedAbort ()
    {
	int outcome = PrepareOutcome.FINISH_ERROR;
	
	if ((objectAddr != null) && (actionHandle != null))
	    outcome = (objectAddr.forgetAction(actionHandle, false, RecordType.ACTIVATION) ? PrepareOutcome.FINISH_OK : PrepareOutcome.FINISH_ERROR);

	return PrepareOutcome.FINISH_ERROR;
    }

    /*
     * nestedCommit does nothing since the passing of the state up to
     * the parent action is handled by the record list merging system.
     * In fact since nested_prepare returns READONLY this function should
     * never actually be called
     */
    
public int nestedCommit ()
    {
	return PrepareOutcome.FINISH_OK;
    }

public int nestedPrepare ()
    {
	return PrepareOutcome.READONLY;
    }

    /*
     * topLevelAbort for Activation records is exactly like a nested
     * abort.
     */

public int topLevelAbort ()
    {
	return nestedAbort();		/* ie same as nested case */
    }

    /*
     * topLevelCommit has little to do for ActivationRecords other
     * than to ensure the object is made PASSIVE
     */

public int topLevelCommit ()
    {
	if ((objectAddr != null) && (actionHandle != null))
	{
	    return (objectAddr.forgetAction(actionHandle, true, RecordType.ACTIVATION) ? PrepareOutcome.FINISH_OK : PrepareOutcome.FINISH_ERROR);
	}

	return PrepareOutcome.FINISH_ERROR;
    }

public int topLevelPrepare ()
    {
	if (objectAddr == null)
	    return PrepareOutcome.PREP_NOTOK;
	else
	    return PrepareOutcome.PREP_OK;
    }

    /*
     * Saving of ActivationRecords is only undertaken during the Prepare
     * phase of the top level 2PC.
     */

public boolean restore_state (InputObjectState os, int v)
    {
	System.err.println("Invocation of ActivationRecord::restore_state for "+
			   type()+" inappropriate - ignored for "+order());
	return false;
    }

public boolean save_state (OutputObjectState os, ObjectType v)
    {
	return true;
    }

public void print (PrintStream strm)
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
	state = ObjectStatus.PASSIVE;
	actionHandle = null;
    }

private StateManager objectAddr;
private BasicAction actionHandle;
private int state;
    
}
