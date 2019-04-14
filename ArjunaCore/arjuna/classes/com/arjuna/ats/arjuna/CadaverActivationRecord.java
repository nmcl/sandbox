/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: CadaverActivationRecord.java,v 1.3 2003/08/11 15:04:12 nmcl Exp $
 */

package com.arjuna.ats.arjuna;

import com.arjuna.ats.arjuna.logging.tsLogger;
import com.arjuna.ats.arjuna.logging.FacilityCode;

import com.arjuna.ats.arjuna.coordinator.*;
import com.arjuna.ats.arjuna.common.*;

import com.arjuna.common.util.logging.*;

/*
 * This constructor is used to create a new instance of an
 * CadaverActivationRecord.
 */

class CadaverActivationRecord extends ActivationRecord
{

    public CadaverActivationRecord (StateManager sm)
    {
	super(ObjectStatus.PASSIVE, sm, null);
	    
	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC, 
				     FacilityCode.FAC_ABSTRACT_REC, 
				     "CadaverActivationRecord::CadaverActivationRecord(" +sm.get_uid()+")");
	}
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
	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
				     FacilityCode.FAC_ABSTRACT_REC, 
				     "CadaverActivationRecord::nestedAbort() for "+get_uid());
	}
	
	return TwoPhaseOutcome.FINISH_OK;
    }
    
    public int nestedCommit ()
    {
	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
				     FacilityCode.FAC_ABSTRACT_REC, 
				     "CadaverActivationRecord::nestedCommit() for "+get_uid());
	}
	
	return TwoPhaseOutcome.FINISH_OK;	
    }
    
    public int nestedPrepare ()
    {
	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
				     FacilityCode.FAC_ABSTRACT_REC, 
				     "CadaverActivationRecord::nestedPrepare() for "+get_uid());
	}
	
	return TwoPhaseOutcome.PREPARE_READONLY;
    }
    
    public int topLevelAbort ()
    {
	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
				     FacilityCode.FAC_ABSTRACT_REC, 
				     "CadaverActivationRecord::topLevelAbort() for "+get_uid());
	}
	
	return TwoPhaseOutcome.FINISH_OK;
    }
    
    public int topLevelCommit ()
    {
	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC, FacilityCode.FAC_ABSTRACT_REC, 
				     "CadaverActivationRecord::topLevelCommit() for "+get_uid());
	}
	
	return TwoPhaseOutcome.FINISH_OK;
    }

    public int topLevelPrepare ()
    {
	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC, FacilityCode.FAC_ABSTRACT_REC, 
				     "CadaverActivationRecord::topLevelPrepare() for "+get_uid());
	}
	
	return TwoPhaseOutcome.PREPARE_READONLY;
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

	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PROTECTED,
				     FacilityCode.FAC_ABSTRACT_REC, 
				     "CadaverActivationRecord::CadaverActivationRecord ()");
	}
    }
    
}
