/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_ServerRestrictedNestedAction.java,v 1.1.8.2.20.1.2.1.2.1 2000/12/13 15:40:25 nmcl Exp $
 */

package com.arjuna.CosTransactions.Interposition.Restricted;

import com.arjuna.OrbCommon.*;
import com.arjuna.ArjunaCore.Thread.ThreadActionData;
import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaCommon.Template.HashList;
import com.arjuna.CosTransactions.*;
import com.arjuna.CosTransactions.Interposition.OTS_ServerControl;
import com.arjuna.CosTransactions.Interposition.Arjuna.*;
import org.omg.CosTransactions.*;

import org.omg.CosTransactions.HeuristicHazard;
import org.omg.CosTransactions.HeuristicMixed;

public class OTS_ServerRestrictedNestedAction extends OTS_ServerNestedAction
{
    
    /*
     * Create local transactions with same ids as remote.
     */

public OTS_ServerRestrictedNestedAction (OTS_ServerControl myControl)
    {
	super(myControl);

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "OTS_ServerRestrictedNestedAction::OTS_ServerRestrictedNestedAction ( "+_theUid+" )");
	}
    }

public final synchronized OTS_ServerControl deepestControl ()
    {
	OTS_ServerRestrictedNestedAction myChild = child();

	if (myChild != null)
	    return myChild.deepestControl();
	else
	    return control();
    }

public final synchronized OTS_ServerRestrictedNestedAction child ()
    {
	OTS_ServerRestrictedNestedAction toReturn = null;
	HashList children = getChildren();
    
	/*
	 * There should be only one child!
	 */
    
	if (children != null)
	{
	    if (children.size() != 1)
	    {
		ErrorStream.stream(ErrorStream.FATAL).println("OTS_ServerRestrictedNestedAction.child - found concurrent transactions!");

		throw new com.arjuna.CosTransactions.OTS_Error("Concurrent children for restricted interposition!");
	    }
	    else
	    {
		toReturn = (OTS_ServerRestrictedNestedAction) children.orderedPop();
	    }
	}
    
	return toReturn;
    }
    
};
