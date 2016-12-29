/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: OTS_ServerTopLevelActionRecovery.java,v 1.1.2.1 2000/05/31 13:53:09 nmcl Exp $
 */

package com.arjuna.CosTransactions.Interposition.Arjuna;

import com.arjuna.ArjunaCommon.Common.Uid;
import com.arjuna.CosTransactions.Interposition.Recovery.InterpositionRecovery;

public class OTS_ServerTopLevelActionRecovery implements InterpositionRecovery
{

public OTS_ServerTopLevelAction create (Uid u)
    {
	return new OTS_ServerTopLevelAction(u);
    }

public boolean recover (Uid u)
    {
	OTS_ServerTopLevelAction act = new OTS_ServerTopLevelAction(u);

	return act.recover();
    }
    
public String type ()
    {
	return "/Resources/Arjuna/OTS_ServerTopLevelAction";
    }

};
