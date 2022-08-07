/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_ServerOSITopLevelAction.javatmpl,v 1.1.8.1.8.1.18.1.2.1 2000/12/13 15:40:25 nmcl Exp $
 */

/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OrbPortability.h,v 1.1 2000/02/25 14:09:59 nmcl Exp $
 */




/*
 * Try to get around the differences between Ansi CPP and
 * K&R cpp with concatenation.
 */


/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: OrbPortability_stdc.h,v 1.2.4.1.2.3.26.1.2.2.4.2 2001/02/02 11:51:52 nmcl Exp $
 */
































































package com.arjuna.CosTransactions.Interposition.OSI;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.CosTransactions.Interposition.Strict.*;
import com.arjuna.CosTransactions.Interposition.*;
import org.omg.CosTransactions.*;

import org.omg.CORBA.SystemException;

public class OTS_ServerOSITopLevelAction extends OTS_ServerStrictTopLevelAction
{
    
    /*
     * The OTS_ServerTopLevelAction is responsible for registering
     * this resource with its parent.
     */

public OTS_ServerOSITopLevelAction (OTS_ServerControl control,
				    boolean doRegister)
    {
	super(control, doRegister);

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "OTS_ServerOSITopLevelAction::OTS_ServerOSITopLevelAction ( OTS_ServerControl, "+doRegister+" )");
	}
    }

/*
 * Will only be called by the remote top-level transaction.
 */


public org.omg.CosTransactions.Vote prepare () throws HeuristicMixed, HeuristicHazard, SystemException

    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "OTS_ServerOSITopLevelAction::prepare for "+_theUid);
	}

	/*
	 * First remove entry for this transaction otid
	 * from map. Have to do it here as we are going
	 * to be deleted by the base class!
	 */
    
	OTIDMap.remove(get_uid());

	return super.prepare();
    }

public void rollback () throws SystemException, HeuristicCommit, HeuristicMixed, HeuristicHazard
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "OTS_ServerOSITopLevelAction::rollback for "+_theUid);
	}

	OTIDMap.remove(get_uid());
    
	super.rollback();
    }

public void commit () throws SystemException, NotPrepared, HeuristicRollback, HeuristicMixed, HeuristicHazard
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "OTS_ServerOSITopLevelAction::commit for "+_theUid);
	}

	OTIDMap.remove(get_uid());
    
	super.commit();
    }

public void forget () throws SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "OTS_ServerOSITopLevelAction::forget for "+_theUid);
	}

	OTIDMap.remove(get_uid());

	super.forget();
    }

/*
 * Just because commit_one_phase is called by the coordinator does not
 * mean that we can use it - we may have many locally registered resources.
 */



public void commit_one_phase () throws HeuristicHazard, SystemException


    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "OTS_ServerOSITopLevelAction::commit_one_phase for "+_theUid);
	}

	OTIDMap.remove(get_uid());

	super.commit_one_phase();
    }

public String type ()
    {
	return "/Resources/Arjuna/OTS_ServerTopLevelAction/OTS_ServerOSITopLevelAction";
    }
    
};
