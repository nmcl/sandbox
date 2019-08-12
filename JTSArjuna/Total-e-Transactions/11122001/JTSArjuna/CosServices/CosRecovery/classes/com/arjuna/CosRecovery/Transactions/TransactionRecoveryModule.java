/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: TransactionRecoveryModule.java,v 1.1.2.1.2.2.38.3.60.2 2002/07/24 10:00:31 msaheb Exp $
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

import java.io.IOException;

/**
 * This class is a plug-in module for the recovery manager.  This is a
 * generic class from which TopLevel and Server transaction recovery
 * modules inherit.
 *
 * This class does not implement {@link com.arjuna.CosRecovery.RecoveryModule}
 * (the plug-in definition) itself - this is left to the subclass.
 */

public abstract class TransactionRecoveryModule
{
    public TransactionRecoveryModule ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_CRASH_RECOVERY,
					       "TransactionRecoveryModule created");
	}

	if (_transactionStore == null)
	{
	    _transactionStore = InitAction.getStore();
	}
    }

    /**
     * This is called periodically by the RecoveryManager
     */
    protected void periodicWorkFirstPass ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_CRASH_RECOVERY,
						 "TransactionRecoveryModule.periodicWorkFirstPass()");
	}
	// Sanity check - make sure we know what type of transaction we're looking for
	if (_transactionType == null)
	{
	    ErrorStream.warning().println("TransactionRecoveryModule: transaction type not set");
	    return;
	}

	// Build a Vector of transaction Uids found in the ObjectStore
	_transactionUidVector = new Vector();
	InputObjectState uids = new InputObjectState();

	boolean anyTransactions = false;
	try
	{
	    if (DebugController.enabled())
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						     FacilityCode.FAC_CRASH_RECOVERY,
						     "TransactionRecoveryModule: scanning for "+_transactionType);
	    }

	    anyTransactions = _transactionStore.allObjUids(_transactionType, uids);
	}
	catch (ObjectStoreException e1)
	{
	    ErrorStream.warning().println("TransactionRecoveryModule"
							    +": Object store exception:" +e1);
	}

	if (anyTransactions)
	{
	    Uid theUid = new Uid(Uid.nullUid());

	    boolean moreUids = true;
	    while (moreUids)
	    {
		try
		{
		    theUid.unpack(uids);

		    if (theUid.equals(Uid.nullUid()))
		    {
			moreUids = false;
		    }
		    else
		    {
			Uid newUid = new Uid (theUid);
			if (DebugController.enabled())
			{
			    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
				 FacilityCode.FAC_CRASH_RECOVERY,
				 "found transaction "+ newUid);
			}
			_transactionUidVector.addElement(newUid);
		    }
		}
		catch (Exception e2)
		{
		    moreUids = false;
		}
	    }
	}
    }

	    /*
	     * We may have caught some transactions in flight that are
	     * going to complete normally. We'll wait a short time
	     * before rechecking if they are still around. If so, we
	     * process them.
	     */
    protected void periodicWorkSecondPass ()
    {
	if (DebugController.enabled()) 
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_CRASH_RECOVERY,
						 "TransactionRecoveryModule.periodicWorkSecondPass()");
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
			DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
				 FacilityCode.FAC_RECOVERY_NORMAL,
				 "Transaction " + currentUid + " still in ActionStore");
		    }

		    recoverTransaction(currentUid);
		} else {
		    // Transaction has gone away - probably completed normally
		    if (DebugController.enabled()) 
		    {
			DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
				 FacilityCode.FAC_CRASH_RECOVERY,
				 "Transaction " +currentUid +" is in state unknown (?).");
		    }
		}
	    }
	    catch (ObjectStoreException e4)
	    {
		// Transaction has gone away - probably completed normally
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
						 "TransactionRecoveryModule.initialise()");
	}
    }

    private void recoverTransaction (Uid tranUid)
	//protected void recoverTransaction (Uid tranUid)
    {
	if (DebugController.enabled())
	    {
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PRIVATE,
						 FacilityCode.FAC_CRASH_RECOVERY,
						 "TransactionRecoveryModule.recoverTransaction("+tranUid+")");
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
	if ( cachedRecoveredTransaction.originalBusy() ) 
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

    protected String	   _transactionType = null;
    //private static ObjectStore _transactionStore = null;

    //private Vector	     _transactionUidVector;

    protected static ObjectStore _transactionStore = null;

    protected Vector	     _transactionUidVector;


    /*
     * Read the properties to set the configurable options
     */
    static
    {
	 // TBD: Inventory.inventory().addToList(new OTS_RecoveryResourceRecordSetup());
    }
};


