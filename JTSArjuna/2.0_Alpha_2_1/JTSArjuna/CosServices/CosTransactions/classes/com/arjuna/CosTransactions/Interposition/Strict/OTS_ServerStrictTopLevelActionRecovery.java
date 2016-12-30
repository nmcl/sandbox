/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: OTS_ServerStrictTopLevelActionRecovery.java,v 1.1.2.1 2000/05/31 13:53:13 nmcl Exp $
 */

package com.arjuna.CosTransactions.Interposition.Strict;

import com.arjuna.ArjunaCommon.Common.Uid;
import com.arjuna.CosTransactions.Interposition.Arjuna.OTS_ServerTopLevelAction;
import com.arjuna.CosTransactions.Interposition.Recovery.InterpositionRecovery;

public class OTS_ServerStrictTopLevelActionRecovery implements InterpositionRecovery
{

public OTS_ServerTopLevelAction create (Uid u)
    {
	return new OTS_ServerStrictTopLevelAction(u);
    }

public boolean recover (Uid u)
    {
	OTS_ServerStrictTopLevelAction act = new OTS_ServerStrictTopLevelAction(u);

	return act.recover();
    }
    
public String type ()
    {
	return "/Resources/Arjuna/OTS_ServerTopLevelAction/OTS_ServerStrictTopLevelAction";
    }

};
