/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CadaverActivationRecord.java,v 1.5 1998/07/06 13:26:16 nmcl Exp $
 */

package com.arjuna.JavaArjuna.ClassLib;

/*
 * This constructor is used to create a new instance of an
 * CadaverActivationRecord.
 */

class CadaverActivationRecord extends ActivationRecord
{

public CadaverActivationRecord (StateManager sm)
    {
	super(ObjectStatus.PASSIVE, sm, null);
    }

public boolean propagateOnAbort ()
    {
	return true;
    }

    /*
     * Supress all atomic action ops for deleted object
     */

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
	return PrepareOutcome.READONLY;
    }

public int topLevelAbort ()
    {
	return PrepareOutcome.FINISH_OK;
    }

public int topLevelCommit ()
    {
	return PrepareOutcome.FINISH_OK;
    }

public int topLevelPrepare ()
    {
	return PrepareOutcome.READONLY;
    }

public String type ()
    {
	return "/StateManager/AbstractRecord/CadaverActivationRecord";
    }

    /*
     * shouldMerge and should_replace are invoked by the record list manager
     * to determine if two records should be merged togethor or if the
     * 'newer' should replace the older.
     * shouldAdd determines if the new record should be added in addition
     * to the existing record and is currently only invoked if both of
     * shouldMerge and shouldReplace return false
     * Default implementations here always return false - ie new records
     * do not override old
     */

public boolean shouldReplace (AbstractRecord ar)
    {
	return  (((order().equals(ar.order())) &&
		  ar.typeIs() == RecordType.ACTIVATION ) ? true : false);
    }

protected CadaverActivationRecord ()
    {
	super();
    }
    
};
