/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ServerTransactionRecoveryModule.java,v 1.1.2.1.110.1.2.1 2002/11/14 13:28:58 nmcl Exp $
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
	//super.periodicWorkSecondPass();

	
	if (DebugController.enabled()) 
	    {
	     DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						  FacilityCode.FAC_CRASH_RECOVERY,
						  "ServerTransactionRecoveryModule.periodicWorkSecondPass()");
	    }
	
	
	// Process the Vector of transaction Uids
	
	Enumeration transactionUidEnum = _transactionUidVector.elements();
	while (transactionUidEnum.hasMoreElements())
	    {
		Uid currentUid = (Uid) transactionUidEnum.nextElement();
		
		try
		    {
		     // Is the intentions list still there? Is this the best way to check?
		     if (_transactionStore.currentState(currentUid, _transactionType) != ObjectStore.OS_UNKNOWN)
		       {
			   if (DebugController.enabled()) 
			    {
			     DebugController.controller().println(DebugLevel.FUNCTIONS, 
								  VisibilityLevel.VIS_PUBLIC,
								  FacilityCode.FAC_RECOVERY_NORMAL,
								  "Transaction " + currentUid + " still in ActionStore");
			    }
				
			   recoverTransaction(currentUid);
		       } else {
			   if (DebugController.enabled()) 
			       {
				   DebugController.controller().println(DebugLevel.FUNCTIONS, 
									VisibilityLevel.VIS_PUBLIC,
									FacilityCode.FAC_CRASH_RECOVERY,
									"Transaction " +currentUid +" is in state unknown (?).");
			       }
			    }
		    }
		catch (ObjectStoreException e4)
		    {
			if (DebugController.enabled()) 
			    {
				DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
								     FacilityCode.FAC_CRASH_RECOVERY,
								     "Transaction " +currentUid +" is not in object store - assumed completed");
			    }
		    }
	    }

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



    protected void recoverTransaction (Uid tranUid)
    { 
       if (DebugController.enabled())
	   {
	       DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PRIVATE,
						    FacilityCode.FAC_CRASH_RECOVERY,
						    "ServerTransactionRecoveryModule.recoverTransaction("+tranUid+")");
	   }
       
       Status currentStatus = Status.StatusUnknown;
       
       CachedRecoveredTransaction cachedRecoveredTransaction = new CachedRecoveredTransaction (tranUid, _transactionType);
       
       currentStatus = cachedRecoveredTransaction.get_status();
       
       if (DebugController.enabled())
	   {
	       DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PRIVATE,
						    FacilityCode.FAC_CRASH_RECOVERY,
						    "Activated transaction "+tranUid+" status = "+OTS_Utility.printStatus(currentStatus));
	   }

       // but first check that the original transaction isn't in mid-flight
       if ( cachedRecoveredTransaction.originalBusy() && (currentStatus != Status.StatusPrepared) ) 
	   {
	       if (DebugController.enabled())
		   {
		       DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PRIVATE,
							    FacilityCode.FAC_CRASH_RECOVERY,
							    "Transaction "+tranUid+" still busy");
		   }
	       return;
	   }
       
       cachedRecoveredTransaction.replayPhase2();
       cachedRecoveredTransaction = null;
    }
	
}


