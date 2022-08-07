/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: CachedRecoveredTransaction.javatmpl,v 1.1.2.1.2.1 2001/02/06 15:43:19 pfurniss Exp $
 */

/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OrbPortability.h,v 1.1 2000/02/25 14:09:59 nmcl Exp $
 */




/*
 * Try to get around the differences between Ansi CPP and
 * K&R cpp with concatenation.
 */


/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: OrbPortability_stdc.h,v 1.2.4.1.2.3.26.1.2.2.4.2 2001/02/02 11:51:52 nmcl Exp $
 */
































































package com.arjuna.CosRecovery.Transactions;

import java.util.Hashtable;
import com.arjuna.ArjunaCommon.Common.*;
import org.omg.CosTransactions.*;
import com.arjuna.CosTransactions.OTS_Utility;
import com.arjuna.CosRecovery.Contact.StatusChecker;

import org.omg.CORBA  .SystemException;

/**
 * Any transaction that is identified by the recovery system to
 * require recovery is manipulated through
 * CachedRecoveredTransactions. CachedRecoveredTransactions delegate
 * through to a static transaction cache which ensures that there is
 * no interference between different threads that may be working with
 * the same transaction 
 * <P>
 * @author Dave Ingham (dave@arjuna.com)
 * @version $Id: CachedRecoveredTransaction.javatmpl,v 1.1.2.1.2.1 2001/02/06 15:43:19 pfurniss Exp $
 * @see TransactionCache
 */

public class CachedRecoveredTransaction
{
    public CachedRecoveredTransaction ( Uid actionUid, String theType )
    {
	_theTransactionUid = new Uid (actionUid);
	_theTransactionType = theType;

	if (DebugController.enabled()) 
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_CRASH_RECOVERY,
						 "CachedRecoveredTransaction created ["
						 +_theTransactionUid+", "+_theTransactionType+"]");
	}
    }
    
    public void finalize ()
    {
	if (DebugController.enabled()) 
	{
	    DebugController.controller().println(DebugLevel.DESTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_CRASH_RECOVERY,
						 "CachedRecoveredTransaction.finalise ["
						 +_theTransactionUid+", "+_theTransactionType+"]");
	}
	_theTransactionUid = null;
	_theTransactionType = null;
    }

    public Uid getTransactionUid()
    {
	if (DebugController.enabled()) 
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_CRASH_RECOVERY,
						 "CachedRecoveredTransaction.getTransactionUid() =" 
						 +_theTransactionUid);
	}
	return _theTransactionUid;
    }

    /**
     * Get the status of the transaction
     */
    public synchronized Status get_status () throws SystemException
    {
	Status theStatus = TransactionCache.get_status(_theTransactionUid, _theTransactionType);
	if (DebugController.enabled()) 
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_CRASH_RECOVERY,
						 "CachedRecoveredTransaction.get_status ["
						 +_theTransactionUid+", "+_theTransactionType+"] = " 
						 +OTS_Utility.printStatus(theStatus));
	}
	return theStatus;
    }

    /* THIS LOGIC IS MISPLACED - it needs a javatmpl */
    boolean /* sync ? */ originalBusy()
    {
	Status originalStatus = getOriginalStatus();
	if (DebugController.enabled()) 
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_CRASH_RECOVERY,
						 "CachedRecoveredTransaction.originalBusy - told status is "
						 + OTS_Utility.printStatus(originalStatus));
	}

	switch ( originalStatus.value() ) {
	    // original process dead or finished with tran
	    case  Status ._StatusNoTransaction  :
		return false;
		
	    // these states can only come from a process that is still alive
	    case  Status ._StatusActive  :
	    case  Status ._StatusMarkedRollback  :
	    case  Status ._StatusPreparing  :
	    case  Status ._StatusCommitting  :
	    case  Status ._StatusRollingBack  :

	    case  Status ._StatusPrepared  :
		return true;
	    
	    // the transaction is apparently still there, but has completed its
	    // phase2. should be safe to redo it (this argument is a bit shaky)
	    case  Status ._StatusCommitted  :
	    case  Status ._StatusRolledBack  :
		return false;
				
	    // this shouldn't happen - assume busy
	    case  Status ._StatusUnknown  :
	    default:
		return true;
	}
    }
    
    /**
     *  what is the status of the transaction in the original process ? (if alive)
     */
    Status /* sync ? */ getOriginalStatus()
    {
	return TransactionCache.getOriginalStatus(_theTransactionUid,_theTransactionType);
    }
    
    /** 
     * Get the recovery status of the transaction
     */
    public int getRecoveryStatus ()
    {
	return TransactionCache.getRecoveryStatus(_theTransactionUid, _theTransactionType);
    }

    /**
     * Add a new resource to a recovered transaction. This is
     * primarily to allow a new resource that has been provided
     * through a replay_completion to be added to the transaction and
     * thereby replacing the original resource that was passed in on
     * register_resource.  
     */
    public void addResourceRecord (Uid rcUid, Resource r)
    {
	if (DebugController.enabled()) 
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_CRASH_RECOVERY,
						 "CachedRecoveredTransaction.addResourceRecord ["
						 +_theTransactionUid+", "+_theTransactionType+"]"
						 +"("+rcUid+")");
	}
	TransactionCache.addResourceRecord(_theTransactionUid, _theTransactionType, rcUid, r);
    }

    /**
     * Replays phase 2 of the transaction.
     */
    public void replayPhase2()
    {	
	if (DebugController.enabled()) 
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_CRASH_RECOVERY,
						 "CachedRecoveredTransaction.replayPhase2 ["
						 +_theTransactionUid+", "+_theTransactionType+"]");
	}
	TransactionCache.replayPhase2(_theTransactionUid, _theTransactionType);
    }

    private Uid    _theTransactionUid = null;
    private String _theTransactionType = null;
}
