/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: RecoveredTransactionReplayer.java,v 1.1.2.1 2000/07/30 20:29:56 ndbi Exp $
 */

package com.arjuna.CosRecovery;

import com.arjuna.ArjunaCommon.Common.*;
import org.omg.CosTransactions.*;

public class RecoveredTransactionReplayer extends Thread
{
    public RecoveredTransactionReplayer( Uid actionUid )
    {
	_actionUid = actionUid;
	_recoveredTransaction = new RecoveredTransaction (_actionUid);
    }
    
    public void finalize () throws Throwable
    {
	super.finalize();
	_recoveredTransaction = null;
    }

    public Status getStatus()
    {
	if (_recoveredTransaction.activated())
	    return _recoveredTransaction.get_status();
	else 
	    return Status.StatusNoTransaction;
    }

    public void swapResource (Uid rcUid, Resource r)
    {
	_recoveredTransaction.addResourceRecord(rcUid, r);
    }
    
    public void replayPhase2Commit()
    {
	start();
    }
    
    public void run()
    {
	if (_recoveredTransaction.activated())
	{
	    _recoveredTransaction.replayPhase2Commit();
	}
	else
	{
	    System.err.println("RecoveredTransactionReplayer: "
			       + "failed to activate transaction " +_actionUid);
	}
	_recoveredTransaction = null;
    }

    private Uid _actionUid = null;
    private RecoveredTransaction _recoveredTransaction = null;
}

    
    
