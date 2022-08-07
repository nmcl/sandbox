/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_ServerRecoveryTopLevelAction.java,v 1.1.2.5.14.1.4.1 2000/12/13 15:40:24 nmcl Exp $
 */

package com.arjuna.CosTransactions.Interposition.Arjuna;

import com.arjuna.OrbCommon.*;
import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaCore.Atomic.PrepareOutcome;
import com.arjuna.ArjunaCore.Atomic.ActionStatus;
import com.arjuna.ArjunaCore.Thread.ThreadActionData;
import com.arjuna.ArjunaCore.Interface.ObjectStore;
import com.arjuna.CosTransactions.*;
import com.arjuna.CosTransactions.Interposition.OTS_ServerControl;
import com.arjuna.CosTransactions.Interposition.OTS_ServerTransaction;
import org.omg.CosTransactions.*;

public class OTS_ServerRecoveryTopLevelAction extends OTS_ServerTopLevelAction
{

public OTS_ServerRecoveryTopLevelAction (OTS_ServerControl control)
    {
	super();

	_theControl = control;

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "OTS_ServerRecoveryTopLevelAction::OTS_ServerRecoveryTopLevelAction ( "+_theUid+" )");
	}
    }

    /*
     * Basically a null-op to prevent us trying to remove
     * this resource from the interposition list. Since it
     * gets created explicitly for crash recovery, it doesn't
     * get added to the list.
     */

protected synchronized void destroyResource ()
    {
	if (!_destroyed)
	{
	    _destroyed = true;
	}
    }

};
