/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: RecoveredTransactionalObject.java,v 1.1.2.3.2.2.14.1.92.1 2002/07/09 16:05:40 nmcl Exp $
 */


package com.arjuna.CosRecovery.TransactionalObjects;

import java.io.IOException;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaCore.Atomic.*;
import com.arjuna.ArjunaCore.Interface.ObjectStore;
import com.arjuna.ArjunaCore.Common.ObjectStoreException;
import com.arjuna.CosTransactions.OTS_Utility;
import org.omg.CosTransactions.Status;

import com.arjuna.CosRecovery.Transactions.TransactionCache;
import com.arjuna.CosRecovery.Contact.*;

import org.omg.CosTransactions.Inactive;

/**
 * TransactionalObject shell instantiated at recovery time. 
 * <p>Instantiated only for 
 * TransactionalObjects that are found (by {@link TORecoveryModule}) to be in an uncommitted
 * (indeterminate) state. The status of the transaction that created the uncommitted state is 
 * determined - if the transaction rolled back, the original state of the TransactionalObject is
 * reinstated. If the transaction rolled back (or is still in progress), no change is made -
 * the completion (including completion in recovery) of the transaction will be applied to 
 * the transactional object (eventually).
 * <p>Instantiation from the ObjectStore ignores all of the TO except for the information
 * in the header 
 * ( see {@link com.arjuna.ArjunaCore.Atomic.StateManager#packHeader StateManager.packHeader}), 
 * which is overridden by this class).
 * <P>
 * @author Peter Furniss (peter.furniss@arjuna.com)
 * @version $Id: RecoveredTransactionalObject.java,v 1.1.2.3.2.2.14.1.92.1 2002/07/09 16:05:40 nmcl Exp $ 
 */
 // does not extend LockManager or StateManager because they are concerned with 
 // activating the committed state, and this is only concerned with the uncommitted
 
public class RecoveredTransactionalObject extends StateManager
{
    Uid	 _ourUid;
    Uid	 _owningTransactionUid;
    Uid	_originalProcessUid;
    ObjectStore _objectStore;
    String	 _type;
    
    RecoveredTransactionalObject ( Uid objectUid, 
		String originalType , ObjectStore objectStore)
    {
	
	_ourUid = objectUid;
	_type = originalType;
	_objectStore = objectStore;
	
	if (DebugController.enabled()) 
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PACKAGE,
				 FacilityCode.FAC_CRASH_RECOVERY,
				 "RecoveredTransactionalObject created for "+ _ourUid);
	}
    }
    
    void replayPhase2()
    {
	if (findHoldingTransaction()) {
	    // there is a transaction holding this in uncommitted state
	    // find out what the Status is
	    //   we have no idea what type of transaction it is, so leave
	    //   that to the cache
	    if (DebugController.enabled())
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
			FacilityCode.FAC_CRASH_RECOVERY,
			"TO held by transaction " + _owningTransactionUid);
	    }

	    Status tranStatus = TransactionCache.getCachedStatus(_owningTransactionUid);
	    if (DebugController.enabled())
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
			FacilityCode.FAC_CRASH_RECOVERY,
			"transaction Status from cache " + OTS_Utility.printStatus(tranStatus));
	    }

	    boolean inactive = false;
	    
	    if (tranStatus == Status.StatusNoTransaction)
	    {
		try
		{
		    // it wasn't in the cache - perhaps it's still awake
		    tranStatus = StatusChecker.get_status(_owningTransactionUid, _originalProcessUid);
		}
		catch (Inactive ex)
		{
		    // tx failed.

		    inactive = true;
		}
		
		if (DebugController.enabled())
		{
		    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
			    FacilityCode.FAC_CRASH_RECOVERY,
			    "transaction Status from original application " + OTS_Utility.stringStatus(tranStatus) + " and inactive: "+inactive);
		}
	    }

	    /* only do anything if we are sure the transaction rolledback
	     *   if it is still in progress in the original application, let that run
	     *   otherwise the transaction should recover and do the commitment
	     */

	    if ((tranStatus == Status.StatusRolledBack) || inactive)
	    {
		rollback();
	    }
	
	    /*       here's a summary from the removed switch - which would require proprocessing
		// these states can only come from a process that is still alive
		// we will let it complete rather than intefere and riskconfuisng things
		Status.StatusActive
		Status.StatusMarkedRollback:
		Status.StatusPreparing:
		Status.StatusCommitting:
		Status.StatusRollingBack:

		// the transaction should be doing its recovery soon, so we'll leave
		// it till then
		Status.StatusPrepared:
		Status.StatusCommitted:
		    
		// definitely rolled back, so we'll do it
		Status.StatusRolledBack:
		// transaction known neither in cache or original - presume rollback
		Status.StatusNoTransaction:
		// this shouldn't happen,
		Status.StatusUnknown:
	    */

	} else {
	    if (DebugController.enabled())
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
			FacilityCode.FAC_CRASH_RECOVERY,
			"RecoveredTransactionalObject.replayPhase2 - cannot find/no holding transaction");
	    }
	}
	
	
    }
    
    /**
     *  Determine which transaction got this into uncommitted state
     *   return true if there is such a transaction
     */
    private boolean findHoldingTransaction()
    {
	InputObjectState uncommittedState;
	
	_originalProcessUid = new Uid(Uid.nullUid());
	
	try
	{
	    uncommittedState = _objectStore.read_uncommitted(_ourUid, _type);
	}
	catch (ObjectStoreException e)
	{
	    if (DebugController.enabled()) 
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PACKAGE,
				     FacilityCode.FAC_CRASH_RECOVERY,
				     "RecoveredTransactionalObject tried to access object store " + e);
		e.printStackTrace(DebugController.controller().getStream());
	    }
	    return false;   // probably
	}

	// get the transaction and original process information from the saved state
	_originalProcessUid = new Uid(Uid.nullUid());
	_owningTransactionUid = new Uid(Uid.nullUid());
	try
	{
	    
	    unpackHeader(uncommittedState, _owningTransactionUid, _originalProcessUid);
	    if (DebugController.enabled())
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
			    FacilityCode.FAC_CRASH_RECOVERY,
			    "RecoveredTransactionalObject::findHoldingTransaction - uid is " +
			    _owningTransactionUid);
	    }
	    return  _owningTransactionUid.notEquals(Uid.nullUid());
	    
	}
	catch (Exception e)
	{
	    if (DebugController.enabled())
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
			    FacilityCode.FAC_CRASH_RECOVERY,
			    "RecoveredTransactionalObject::findTransactionUid - exception " + e);
	    }
	}

	return false;
    }
    
    private void rollback()
    {
	try {
	    _objectStore.remove_uncommitted(_ourUid, _type);
	} catch ( ObjectStoreException e) {
	    ErrorStream.warning().println("Object store exception on removing uncommitted" + e);
	}
    }
    
	
}
