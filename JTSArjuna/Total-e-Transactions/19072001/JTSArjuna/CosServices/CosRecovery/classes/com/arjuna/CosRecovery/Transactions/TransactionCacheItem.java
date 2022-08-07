/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: TransactionCacheItem.java,v 1.1.2.1.2.2 2001/02/09 10:22:09 pfurniss Exp $
 */


package com.arjuna.CosRecovery.Transactions;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.CosTransactions.OTS_Transaction;
import com.arjuna.CosTransactions.Interposition.OTS_ServerTransaction;
import org.omg.CosTransactions.*;


/**
 * Would be an inner class of TransactionCache, except it is used by a static
 * method of TransactionCache.
 * (handles the creation of different kinds of transaction
 */
class TransactionCacheItem
{
    private Uid		      _uid;
    private RecoveringTransaction _transaction;
    private String		  _type;
    private int		      _attempts;    
    private Status		  _knownStatus;
    
    TransactionCacheItem(Uid uid,String type)
    {
	_uid = new Uid(uid);   // copy as usual (I wonder ... )
	_type = type;
	_attempts=0;
	// NoTransaction is used to mean that the status is not
	// actually known
	_knownStatus = Status.StatusNoTransaction;
	loadTransaction(true);
    }
    
    /**
     *  activate the transaction. Allow for the possibility that the transaction
     *  has been assumed complete since it was last heard of
     */
    private boolean loadTransaction(boolean firstLoad)
    {
	if (_type.equals(OTS_Transaction.typeName()))
	{
	    _transaction = new RecoveredTransaction(_uid);
	    if ( firstLoad && _transaction.getRecoveryStatus() == RecoveryStatus.ACTIVATE_FAILED) {
		//  this is a previously unknown transaction, and its not in the store
		//  perhaps it was previously assumed complete
		RecoveringTransaction assumed = new AssumedCompleteTransaction(_uid);
		if ( assumed.getRecoveryStatus() != RecoveryStatus.ACTIVATE_FAILED ) {
		    if (DebugController.enabled())
		    {
			DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
				    FacilityCode.FAC_CRASH_RECOVERY,
				    "Transaction " + _uid + " previously assumed complete");
		    }
		    _transaction = assumed;
		    _type = _transaction.type();
		} 
	    }
	}
	else if (_type.equals(OTS_ServerTransaction.typeName()))
	{
	    _transaction = new RecoveredServerTransaction(_uid);
	    if ( firstLoad && _transaction.getRecoveryStatus() == RecoveryStatus.ACTIVATE_FAILED) {
		//  this is a previously unknown transaction, and its not in the store
		//  perhaps it was previously assumed complete
		RecoveringTransaction assumed = new AssumedCompleteServerTransaction(_uid);
		if ( assumed.getRecoveryStatus() != RecoveryStatus.ACTIVATE_FAILED ) {
		    if (DebugController.enabled())
		    {
			DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
				    FacilityCode.FAC_CRASH_RECOVERY,
				    "Transaction " + _uid + " previously assumed complete");
		    }
		    _transaction = assumed;
		    _type = _transaction.type();
		} 
	    }
	}
	else if (_type.equals(AssumedCompleteTransaction.typeName()))
	{
	    _transaction = new AssumedCompleteTransaction(_uid);
	}
	else if (_type.equals(AssumedCompleteServerTransaction.typeName()))
	{
	    _transaction = new AssumedCompleteServerTransaction(_uid);
	}
	else
	{
	    ErrorStream.warning().println("TransactionCacheItem.loadTransaction - unknown type: "+_type);
	    _transaction = null;
	    return false;
	}
	return true;
    }

    /**
     *  the transaction type has been changed
     */
    void updateType()
    {
	_type = _transaction.type();
    }

    /**
     * forget the activated copy of the transaction. Makes it subject to garbage collecting
     */    
    void clearTransaction()
    {
	_transaction = null;
    }
    
    RecoveringTransaction transaction()
    {
	return _transaction;
    }
    
    /**
     *  Make sure the transaction is freshly activated and hasn't been replayed
     *   assumed to be called from code synchronized on the TransactionCacheItem
     */
    
    RecoveringTransaction freshTransaction()
    {
	if (_transaction == null || 
		_transaction.getRecoveryStatus() == RecoveryStatus.REPLAYED)
	{
	    // The transaction has been replayed, but they want a fresh one
	    // Destroy this transaction and create a new
	    // one thereby reactivating it.
	    // Reactivate as the appropriate transaction class
	    loadTransaction(false);
	}
	return _transaction;
    }
    
    /**
     * keep (and return) a counter - used to record repeated failures
     */
    int countAttempts()
    {
	return _attempts++;
    }
    
    /**
     * reset the attempt account
     */
    void resetAttemptCount()
    {
	_attempts = 0;
    }
    
    /**
     * mutator for known status
     */
    void setStatus(Status status)
    {
	_knownStatus = status;
    }
    
    /**
     * mutator for known status
     */
    Status getStatus()
    {
	return _knownStatus;
    }
}
