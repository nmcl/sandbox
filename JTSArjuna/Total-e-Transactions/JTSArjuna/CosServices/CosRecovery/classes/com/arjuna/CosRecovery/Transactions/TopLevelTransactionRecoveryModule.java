/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: TopLevelTransactionRecoveryModule.java,v 1.1.2.1 2001/01/29 18:38:08 pfurniss Exp $
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


// todo - make add a protected getTransactionType() method

/**
 * This class is a plug-in module for the recovery manager.
 * It is responsible for the recovery of server transactions
 */

public class TopLevelTransactionRecoveryModule extends TransactionRecoveryModule
		    implements RecoveryModule
{
    public TopLevelTransactionRecoveryModule ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_CRASH_RECOVERY,
						 "TopLevelTransactionRecoveryModule created");
	}

	// Set the transaction type that this module wants to recover
	if (_transactionType == null)
	    _transactionType = OTS_Transaction.typeName();
    }

    public void finalize () throws Throwable
    {
	super.finalize();

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.DESTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_CRASH_RECOVERY, "TopLevelTransactionRecoveryModule");
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
						 "TopLevelTransactionRecoveryModule - first pass");
	}
	super.periodicWorkFirstPass();
    }

    public void periodicWorkSecondPass ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_RECOVERY_NORMAL,
						 "TopLevelTransactionRecoveryModule - second pass");
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
						 "TopLevelTransactionRecoveryModule.initialise()");
	}
	super.initialise();
    }

};


