/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: LockRecord.java,v 1.1.4.1.4.1.2.1 2001/01/04 12:35:20 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Atomic;

import com.arjuna.ArjunaCore.ArjunaCoreNames;
import com.arjuna.ArjunaCommon.Common.*;
import java.io.*;

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
	managerType = lm.type();
    }
    
public LockRecord (LockManager lm, boolean rdOnly)
    {
	super(lm.get_uid(), lm.type(), ObjectType.ANDPERSISTENT);

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 (FacilityCode.FAC_CONCURRENCY_CONTROL | FacilityCode.FAC_ABSTRACT_REC),
						 "LockRecord::LockRecord("+lm.get_uid()+", "
						 +(readOnly ? "PREP_READONLY" : "WRITEABLE")+")");
	}

	managerAddress = lm;
	readOnly = rdOnly;
	managerType = lm.type();
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
	ErrorStream.stream(ErrorStream.WARNING).println("LockRecord::set_value() called illegally");
    }

public int nestedAbort ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 (FacilityCode.FAC_CONCURRENCY_CONTROL | FacilityCode.FAC_ABSTRACT_REC),
						 "LockRecord::nestedAbort() for "+order());
	}

	/* default constructor problem. */

	if (managerAddress == null)
	    return PrepareOutcome.FINISH_ERROR;

	BasicAction currAct = BasicAction.Current();
	
	if (currAct != null)
	{
	    if (!managerAddress.releaseAll(currAct.get_uid()))
	    {
		ErrorStream.stream(ErrorStream.WARNING).println("LockRecord - release failed for action "+currAct.get_uid());

		return PrepareOutcome.FINISH_ERROR;
	    }
	}
	else
	    ErrorStream.stream(ErrorStream.WARNING).println("LockRecord::nestedAbort - no current action");
    
	return PrepareOutcome.FINISH_OK;
    }

public int nestedCommit ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 (FacilityCode.FAC_CONCURRENCY_CONTROL | FacilityCode.FAC_ABSTRACT_REC),
						 "LockRecord::nestedCommit() for "+order());
	}

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
	    ErrorStream.stream(ErrorStream.WARNING).println("LockRecord::nestedCommit - no current action");
	
	return PrepareOutcome.FINISH_OK;
    }

public int nestedPrepare ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 (FacilityCode.FAC_CONCURRENCY_CONTROL | FacilityCode.FAC_ABSTRACT_REC),
						 "LockRecord::nestedPrepare() for "+order());
	}

	return PrepareOutcome.PREP_OK;
    }

public int topLevelAbort ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 (FacilityCode.FAC_CONCURRENCY_CONTROL | FacilityCode.FAC_ABSTRACT_REC),
						 "LockRecord::topLevelAbort() for "+order());
	}

	return nestedAbort();
    }

public int topLevelCommit ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 (FacilityCode.FAC_CONCURRENCY_CONTROL | FacilityCode.FAC_ABSTRACT_REC),
						 "LockRecord::topLevelCommit() for "+order());
	}

	/* default constructor problem. */

	if (managerAddress == null)
	    return PrepareOutcome.FINISH_ERROR;

	BasicAction currAct = BasicAction.Current();

	if (currAct != null)
	{
	    if (!managerAddress.releaseAll(currAct.get_uid()))
	    {
		ErrorStream.stream(ErrorStream.WARNING).println("LockRecord - release failed for action "+currAct.get_uid());

		return PrepareOutcome.FINISH_ERROR;
	    }
	}
	else
	    ErrorStream.stream(ErrorStream.WARNING).println("LockRecord::topLevelCommit - no current action");

	return PrepareOutcome.FINISH_OK;
    }

public int topLevelPrepare ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 (FacilityCode.FAC_CONCURRENCY_CONTROL | FacilityCode.FAC_ABSTRACT_REC),
						 "LockRecord::topLevelPrepare() for "+order());
	}

	if (readOnly)
	{
	    if (topLevelCommit() == PrepareOutcome.FINISH_OK)
		return PrepareOutcome.PREP_READONLY;
	    else
		return PrepareOutcome.PREP_NOTOK;
	}
    
	return PrepareOutcome.PREP_OK;
    }

public String toString ()
    {
	StringWriter strm = new StringWriter();

	print(new PrintWriter(strm));
	
	return strm.toString();
    }
    
public void print (PrintWriter strm)
    {
	super.print(strm);
	strm.println("Lock Recordn");
    }

    /*
     * restore_state and save_state for LockRecords doesn't generally
     * apply due to object pointers.
     */

public boolean restore_state (InputObjectState o, int t)
    {
	ErrorStream.stream(ErrorStream.WARNING).println("Invocation of LockRecord::restore_state for "+type()+"inappropriate - ignored for "+order());
	     
	return false;
    }

public boolean save_state (OutputObjectState o, int t)
    {
	return true;
    }

public String type ()
    {
	return "/StateManager/AbstractRecord/LockRecord";
    }

public final boolean isReadOnly ()
    {
	return readOnly;
    }

public final String lockType ()
    {
	return managerType;
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

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PROTECTED,
						 (FacilityCode.FAC_CONCURRENCY_CONTROL | FacilityCode.FAC_ABSTRACT_REC),
						 "LockRecord::LockRecord()");
	}

	managerAddress = null;
	readOnly = false;
	managerType = null;
    }
    
private LockManager managerAddress;
private boolean     readOnly;
private String      managerType;
    
}
