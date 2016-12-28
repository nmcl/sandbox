/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: RecoveredTransaction.java,v 1.1.2.2 2000/07/30 20:26:56 ndbi Exp $
 */

package com.arjuna.CosRecovery;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaLite.Atomic.*;
import com.arjuna.CosTransactions.OTS_Transaction;
import org.omg.CosTransactions.*;

public class RecoveredTransaction extends OTS_Transaction
{
    public RecoveredTransaction ( Uid actionUid )
    {
	super(actionUid);
	_activated = activate();
    }

    public boolean activated ()
    {
	return _activated;
    }

    public void addResourceRecord (Uid rcUid, Resource r)
    {
	Coordinator coord = null;
	AbstractRecord corbaRec = createOTSRecord(true, r, coord, rcUid);
	addRecord(corbaRec);
    }

    public void replayPhase2Commit()
    {
	if (DebugController.enabled()) 
	    System.out.println("RecoveredTransaction.replayPhase2Commit("+get_uid()+")");

	phase2Commit(_reportHeuristics);
    }

    private boolean _activated;
    private boolean _reportHeuristics = false;
}

    
    
