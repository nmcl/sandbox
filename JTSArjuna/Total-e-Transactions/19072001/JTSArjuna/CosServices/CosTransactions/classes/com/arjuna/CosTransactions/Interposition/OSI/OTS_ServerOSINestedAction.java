/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_ServerOSINestedAction.javatmpl,v 1.1.8.1.24.1.2.1 2000/12/13 15:40:25 nmcl Exp $
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

public class OTS_ServerOSINestedAction extends OTS_ServerStrictNestedAction
{
    
    /*
     * Create local transactions with same ids as remote.
     * The base class is responsible for registering this resource
     * with its parent.
     */
    
public OTS_ServerOSINestedAction (OTS_ServerControl control,
				   boolean doRegister)
    {
	super(control, doRegister);

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "OTS_ServerOSINestedAction::OTS_ServerOSINestedAction ( "+_theUid+" )");
	}
    }

/*
 * Since we may be called multiple times if we are nested and are propagated
 * to our parents, we remember the initial response and return it subsequently.
 */

public void commit_subtransaction (Coordinator parent) throws SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "OTS_ServerOSINestedAction::commit_subtransaction :"+_theUid);
	}

	/*
	 * First remove entry for this transaction otid
	 * from map. Have to do it here as we are going
	 * to be deleted by the base class!
	 */
    
	OTIDMap.remove(get_uid());
    
	super.commit_subtransaction(parent);
    }

public void rollback_subtransaction () throws SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "OTS_ServerOSINestedAction::rollback_subtransaction :"+_theUid);
	}

	OTIDMap.remove(get_uid());
    
	super.rollback_subtransaction();
    }
 
};
