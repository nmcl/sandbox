/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: TopLevelTransactionRecoveryModule.java,v 1.1.2.1 2000/07/27 16:12:11 ndbi Exp $
 */

package com.arjuna.CosRecovery;

import org.omg.CosTransactions.*;

import java.util.*;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaLite.Interface.ObjectStore;
import com.arjuna.ArjunaLite.*;
import com.arjuna.ArjunaLite.Common.*;
import com.arjuna.ArjunaLite.Atomic.*;
import com.arjuna.CosTransactions.*;

import java.io.IOException;

/**
 * This class is a plug-in module for the recovery manager.
 * It is responsible for the recovery of top-level transactions
 */

public class TopLevelTransactionRecoveryModule implements RecoveryModule
{
    public TopLevelTransactionRecoveryModule ()
    {
	if (DebugController.enabled()) 
	    System.out.println("TopLevelTransactionRecoveryModule created");

	if (transactionStore == null)
	{
	    BasicAction tmpAction = new BasicAction();
	    transactionStore = tmpAction.store();
	}
    }

    public void periodicWork ()
    {
	if (DebugController.enabled()) 
	    System.out.println("TopLevelTransactionRecoveryModule.periodicWork()");

	// Build a Vector of transaction Uids found in the ObjectStore

	Vector transactionUidVector = new Vector();
	InputObjectState uids = new InputObjectState();

	boolean anyTransactions = false;
	try 
	{
	    anyTransactions = transactionStore.allObjUids(OTSTransactionType, uids);
	}
	catch (ObjectStoreException e1)
	{
	    System.err.println("TopLevelTransactionRecoveryModule"
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
			transactionUidVector.addElement(newUid);
		    }
		}
		catch (Exception e2)
		{
		    moreUids = false;
		}
	    }

	    // Process the Vector of transaction Uids
	    Enumeration transactionUidEnum = transactionUidVector.elements();
	    while (transactionUidEnum.hasMoreElements())
	    {
		Uid currentUid = (Uid) transactionUidEnum.nextElement();
		recoverTransaction(currentUid);
	    }
	}
    }
    
    protected void initialise ()
    {
	if (DebugController.enabled()) 
	    System.out.println("TopLevelTransactionRecoveryModule.initialise()");
    }

    private void recoverTransaction(Uid tranUid)
    {
	if (DebugController.enabled()) 
	    System.out.println("TopLevelTransactionRecoveryModule.recoverTransaction("+tranUid+")");

	Status currentStatus = Status.StatusUnknown;

	RecoveredTransaction recoveredTransaction = new RecoveredTransaction (tranUid);

	if (recoveredTransaction.activated())
	{
	    currentStatus = recoveredTransaction.get_status();
	}
	else
	{
	    System.err.println("TopLevelTransactionRecoveryModule: "
			       + "failed to activate transaction " +tranUid);
	}

	if (DebugController.enabled())
	    System.out.println("Recovered transaction "+tranUid+" status = "+currentStatus);

	recoveredTransaction.replayPhase2Commit();

	recoveredTransaction = null;
    }

    // TODO - add a method to OTS_Transaction to get this
    static String OTSTransactionType  = "/StateManager/BasicAction/OTS_Transaction";
    static ObjectStore transactionStore = null;
    
};


