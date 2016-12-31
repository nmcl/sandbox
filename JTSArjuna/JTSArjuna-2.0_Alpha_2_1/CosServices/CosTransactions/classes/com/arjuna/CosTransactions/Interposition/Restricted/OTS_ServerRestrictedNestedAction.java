/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_ServerRestrictedNestedAction.java,v 1.1 2000/02/25 14:06:29 nmcl Exp $
 */

package com.arjuna.CosTransactions.Interposition.Restricted;

import com.arjuna.OrbCommon.*;
import com.arjuna.ArjunaLite.Thread.ThreadActionData;
import com.arjuna.ArjunaCommon.Common.Uid;
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
		System.err.println("FATAL - OTS_ServerRestrictedNestedAction.child - found concurrent transactions!");
		System.exit(1);
	    }
	    else
	    {
		toReturn = (OTS_ServerRestrictedNestedAction) children.orderedPop();
	    }
	}
    
	return toReturn;
    }
    
};
