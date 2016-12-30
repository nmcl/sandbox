/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: OTS_ServerRestrictedTopLevelActionRecovery.java,v 1.1.2.1 2000/05/31 13:53:12 nmcl Exp $
 */

package com.arjuna.CosTransactions.Interposition.Restricted;

import com.arjuna.ArjunaCommon.Common.Uid;
import com.arjuna.CosTransactions.Interposition.Arjuna.OTS_ServerTopLevelAction;
import com.arjuna.CosTransactions.Interposition.Recovery.InterpositionRecovery;

public class OTS_ServerRestrictedTopLevelActionRecovery implements InterpositionRecovery
{

public OTS_ServerTopLevelAction create (Uid u)
    {
	return new OTS_ServerRestrictedTopLevelAction(u);
    }
    
public boolean recover (Uid u)
    {
	OTS_ServerRestrictedTopLevelAction act = new OTS_ServerRestrictedTopLevelAction(u);

	return act.recover();
    }

public String type ()
    {
	return "/Resources/Arjuna/OTS_ServerTopLevelAction/OTS_ServerRestrictedTopLevelAction";
    }

};
