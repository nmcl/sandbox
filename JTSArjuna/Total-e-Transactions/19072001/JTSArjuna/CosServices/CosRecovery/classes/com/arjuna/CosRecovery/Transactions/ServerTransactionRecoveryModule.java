/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ServerTransactionRecoveryModule.java,v 1.1.2.1 2001/01/29 18:38:09 pfurniss Exp $
 */

package com.arjuna.CosRecovery.Transactions;

import org.omg.CosTransactions.*;

import java.util.*;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaCore.Interface.ObjectStore;
import com.arjuna.ArjunaCore.*;
import com.arjuna.ArjunaCore.Common.*;
import com.arjuna.ArjunaCore.Atomic.*;
import com.arjuna.CosTransactions.*;
import com.arjuna.CosTransactions.Interposition.OTS_ServerTransaction;
import com.arjuna.CosRecovery.RecoveryModule;

import java.io.IOException;

/**
 * This class is a plug-in module for the recovery manager.
 * It is responsible for the recovery of server transactions
 */

public class ServerTransactionRecoveryModule extends TransactionRecoveryModule
		implements RecoveryModule
{
    public ServerTransactionRecoveryModule ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_CRASH_RECOVERY,
					       "ServerTransactionRecoveryModule created");
	}

	if (_transactionType == null)
	    _transactionType = OTS_ServerTransaction.typeName();
    }

    public void finalize () throws Throwable
    {
	super.finalize();

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.DESTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_CRASH_RECOVERY, "ServerTransactionRecoveryModule");
	}
    }

    /**
     * This is called periodically by the RecoveryManager
     */
    public void periodicWorkFirstPass ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_RECOVERY_NORMAL,
						 "ServerTransactionRecoveryModule - first pass");
	}
	super.periodicWorkFirstPass();
    }

    public void periodicWorkSecondPass ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_RECOVERY_NORMAL,
						 "ServerTransactionRecoveryModule - second pass");
	}
	super.periodicWorkSecondPass();
    }

    /**
     * Set-up routine
     */
    protected void initialise ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PROTECTED,
						 FacilityCode.FAC_CRASH_RECOVERY,
						 "ServerTransactionRecoveryModule.initialise()");
	}
	super.initialise();
    }

};


