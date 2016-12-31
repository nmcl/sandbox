/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: OTS_ServerRestrictedTopLevelAction.java,v 1.1 2000/02/25 14:06:30 nmcl Exp $
 */

package com.arjuna.CosTransactions.Interposition.Restricted;

import com.arjuna.OrbCommon.*;
import com.arjuna.ArjunaCommon.Common.Uid;
import com.arjuna.ArjunaCommon.Template.HashList;
import com.arjuna.ArjunaLite.Atomic.PrepareOutcome;
import com.arjuna.ArjunaLite.Atomic.ActionStatus;
import com.arjuna.ArjunaLite.Thread.ThreadActionData;
import com.arjuna.CosTransactions.*;
import com.arjuna.CosTransactions.Interposition.OTS_ServerControl;
import com.arjuna.CosTransactions.Interposition.Arjuna.OTS_ServerTopLevelAction;
import org.omg.CosTransactions.*;

/*
 * This looks like an atomic action, but is not actually derived from
 * BasicAction or OTS_Transaction. This is because of the way in which the
 * OTS creates and manipulates transactions.
 *
 * As with OTS_Transaction, we only create actions here, and do not associated
 * these contexts with any thread. We do the association later.
 *
 * If we were to do the creation via a suitably modified current interface
 * then the thread association would be done for us automatically, and we
 * would not have to call resume at all.
 *
 * This is a top-level action proxy.
 */

public class OTS_ServerRestrictedTopLevelAction extends OTS_ServerTopLevelAction
{

public OTS_ServerRestrictedTopLevelAction (OTS_ServerControl myControl)
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
		System.err.println("FATAL - OTS_ServerRestrictedTopLevelAction.child - found concurrent transactions!");
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


