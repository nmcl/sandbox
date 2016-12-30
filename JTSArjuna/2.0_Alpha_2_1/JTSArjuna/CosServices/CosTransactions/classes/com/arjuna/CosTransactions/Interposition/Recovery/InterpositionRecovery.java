/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: InterpositionRecovery.java,v 1.1.2.1 2000/05/31 13:53:11 nmcl Exp $
 */

package com.arjuna.CosTransactions.Interposition.Recovery;

import com.arjuna.ArjunaCommon.Common.Uid;
import com.arjuna.CosTransactions.Interposition.Arjuna.OTS_ServerTopLevelAction;

public interface InterpositionRecovery
{

public OTS_ServerTopLevelAction create (Uid u);
public boolean recover (Uid u);
public String type ();
    
}
