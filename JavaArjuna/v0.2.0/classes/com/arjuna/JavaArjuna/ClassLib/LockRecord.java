/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LockRecord.java,v 1.9 1998/07/06 13:26:24 nmcl Exp $
 */

package com.arjuna.JavaArjuna.ClassLib;

import com.arjuna.JavaArjuna.Common.*;
import java.io.PrintStream;

/*
 * Default visibility.
 */

class LockRecord extends AbstractRecord
{

public LockRecord (LockManager lm)
    {
	super(lm.get_uid(), lm.type(), ObjectType.ANDPERSISTENT);

	managerAddress = lm;
	readOnly = false;
    }
    
public LockRecord (LockManager lm, boolean rdOnly)
    {
	super(lm.get_uid(), lm.type(), ObjectType.ANDPERSISTENT);

	managerAddress = lm;
	readOnly = rdOnly;
    }

public int typeIs ()
    {
	return RecordType.LOCK;
    }

public Object value ()
    {
	return (Object) managerAddress;
    }

public void setValue (Object o)
    {
	System.err.println("LockRecord::set_value() called illegally");
    }

public int nestedAbort ()
    {
	/* default constructor problem. */

	if (managerAddress == null)
	    return PrepareOutcome.FINISH_ERROR;

	BasicAction currAct = BasicAction.Current();
	
	if (currAct != null)
	{
	    if (!managerAddress.releaseAll(currAct.get_uid()))
	    {
		System.err.println("Lock release failed for action "+currAct.get_uid());
		return PrepareOutcome.FINISH_ERROR;
	    }
	}
	else
	    System.err.println("LockRecord::nestedAbort - no current action");
    
	return PrepareOutcome.FINISH_OK;
    }

public int nestedCommit ()
    {
	/* default constructor problem. */

	if (managerAddress == null)
	    return PrepareOutcome.FINISH_ERROR;

	BasicAction currAct = BasicAction.Current();
	
	if (currAct != null)
	{
	    return (managerAddress.propagate(currAct.get_uid(),
					     currAct.parent().get_uid()) ? PrepareOutcome.FINISH_OK : PrepareOutcome.FINISH_ERROR);
	}
	else
	    System.err.println("LockRecord::nestedCommit - no current action");
	
	return PrepareOutcome.FINISH_OK;
    }

public int nestedPrepare ()
    {
	return PrepareOutcome.PREP_OK;
    }

public int topLevelAbort ()
    {
	return nestedAbort();
    }

public int topLevelCommit ()
    {
	/* default constructor problem. */

	if (managerAddress == null)
	    return PrepareOutcome.FINISH_ERROR;

	BasicAction currAct = BasicAction.Current();
	
	if (currAct != null)
	{
	    if (!managerAddress.releaseAll(currAct.get_uid()))
	    {
		System.err.println("Lock release failed for action "+currAct.get_uid());
		return PrepareOutcome.FINISH_ERROR;
	    }
	}
	else
	    System.err.println("LockRecord::topLevelCommit - no current action");

	return PrepareOutcome.FINISH_OK;
    }

public int topLevelPrepare ()
    {
	if (readOnly)
	{
	    if (topLevelCommit() == PrepareOutcome.FINISH_OK)
		return PrepareOutcome.READONLY;
	    else
		return PrepareOutcome.PREP_NOTOK;
	}
    
	return PrepareOutcome.PREP_OK;
    }

public void print (PrintStream strm)
    {
	super.print(strm);
	strm.println("Lock Recordn");
    }

    /*
     * restroe_state and save_state for LockRecords doesn't generally
     * apply due to object pointers.
     */

public boolean restore_state (InputObjectState o, int t)
    {
	System.err.println("Invocation of LockRecord::restore_state for "+type()+"inappropriate - ignored for "+order());
	     
	return false;
    }

public boolean save_state (OutputObjectState o, int t)
    {
	return true;
    }

public boolean isReadOnly ()
    {
	return readOnly;
    }
        
public String type ()
    {
	return "/StateManager/AbstractRecord/LockRecord";
    }

public void merge (AbstractRecord a)
    {
    }

public void alter (AbstractRecord a)
    {
    }

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

public boolean shouldReplace (AbstractRecord ar)
    {
	if ((order().equals(ar.order())) && typeIs() == ar.typeIs())
	{
	    /*
	     * The first test should ensure that ar is a LockRecord.
	     */
	    
	    if (((LockRecord) ar).isReadOnly() && !readOnly)
		return true;
	}
	
	return false;
    }
    
protected LockRecord ()
    {
	super();

	managerAddress = null;
	readOnly = false;
    }
    
private LockManager managerAddress;
private boolean readOnly;

}
