/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: OTS_Transaction.javatmpl,v 1.7.2.1.4.4.2.1.2.6.6.7.6.1.2.5.2.4.2.3.4.1.2.1.16.2.14.1 2001/07/19 14:10:04 nmcl Exp $
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































































/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: CosTranNames.h,v 1.1.42.1 2001/03/20 16:24:47 nmcl Exp $
 */




/*
 * Simple renaming macros for portability.
 *
 * Slightly different from C++ version in that we don't need the
 * typedefs. However, we do need some mapping for OTS exceptions which
 * should now be system exceptions.
 */

/*
 * CosTransaction module exceptions
 */






/*
 * We need to check this. WRONG_TRANSACTION is supposed to
 * be a system exception (in the latest draft of the OTS),
 * but two ORBs have it as a user exception.
 */





/**/




package com.arjuna.CosTransactions;

import com.arjuna.OrbCommon.*;
import com.arjuna.ArjunaCore.Atomic.BasicAction;
import com.arjuna.ArjunaCore.Atomic.ActionStatus;
import com.arjuna.ArjunaCore.Atomic.AbstractRecord;
import com.arjuna.ArjunaCore.Atomic.AddOutcome;
import com.arjuna.ArjunaCore.Atomic.CheckedAction;
import com.arjuna.ArjunaCore.Interface.ObjectStore;
import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaCommon.Template.*;
import com.arjuna.CosTransactions.ArjunaOTS.Arjuna;
import org.omg.CosTransactions.*;
import com.arjuna.ArjunaOTS.*;
import org.omg.CORBA.CompletionStatus;
import java.util.Enumeration;

import com.arjuna.ArjunaCore.Common.ObjectStoreException;
import org.omg.CORBA.SystemException;
import org.omg.CORBA.BAD_PARAM;
import org.omg.CORBA.UNKNOWN;
import org.omg.CORBA.BAD_OPERATION;
import org.omg.CORBA.NO_PERMISSION;
import org.omg.CORBA.NO_IMPLEMENT;

/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: CosTranExceptions.h,v 1.1.2.1.28.1.14.1 2001/03/20 16:24:46 nmcl Exp $
 */






/*
 * An attempt at a portable way of including the OTS exceptions,
 * which are either mapped as system exceptions or user exceptions,
 * depending upon the Orb.
 */



import org.omg.CosTransactions.WrongTransaction;

import org.omg.CORBA.INVALID_TRANSACTION;
import org.omg.CORBA.TRANSACTION_REQUIRED;
import org.omg.CORBA.TRANSACTION_ROLLEDBACK;





/**
 * OTS implementation class.
 *
 * Implements both the Coordinator & Terminator interfaces of OTS
 * as a single class.
 *
 * Note, because Java does not support multiple inheritance we
 * must make use of the tie facility (uuuggghhhh!!!!)
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: OTS_Transaction.javatmpl,v 1.7.2.1.4.4.2.1.2.6.6.7.6.1.2.5.2.4.2.3.4.1.2.1.16.2.14.1 2001/07/19 14:10:04 nmcl Exp $
 * @since JTS 1.0.
 */

public class OTS_Transaction extends com.arjuna.ArjunaCore.Atomic.BasicAction implements  com.arjuna.ArjunaOTS .ArjunaTransactionOperations 
{

    /*
     * Can't do READY_IMPL_ in constructors since this class is
     * a TIE and so isn't a CORBA object.
     */
    
public OTS_Transaction (Control myParent)
    {
	this(myParent, null);
    }
    
public OTS_Transaction (Control myParent, OTS_Transaction parent)
    {
	super();

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_OTS, "OTS_Transaction Begin for < "+get_uid()+" , "+((parent != null) ? parent.get_uid() : Uid.nullUid())+" >");
	}
	
	parentTransaction = parent;
	controlHandle = null;	
	parentHandle = myParent;
	currentStatus = org.omg.CosTransactions.Status.StatusUnknown;

	rootAction = null;
	_synchs = null;

	super.Begin(parent);

	/*
	 * Add uid of this action to parent.
	 */

	if (parent != null)
	    parent.addChildAction(this);

	currentStatus = determineStatus(this);
	rootAction = this;

	/*
	 * Do this once to avoid overhead.
	 */

	hashCode = get_uid().hashCode();

	if (parent != null)
	{
	    while ((rootAction.parent()) != null)
		rootAction = rootAction.parent();
    
	    topLevelHashCode = rootAction.get_uid().hashCode();
	}
	else
	    topLevelHashCode = hashCode;

	if (OTS_Transaction._checkedTransactions)
	{
	    /*
	     * Fully checked transactions only allow the thread which began the
	     * transaction to terminate it. We get the id of the beginning
	     * thread here.
	     *
	     * The spec. says nothing about crash recovery, so we better assume
	     * it can always complete a transaction!
	     *
	     * If the creating thread dies before terminating the transaction
	     * then we have a problem. Requires change to Thread class to abort
	     * outstanding transactions in this case.
	     *
	     * Also, transaction timeouts cause abortion of the transaction by
	     * a *different* thread! This must work even in the presence of
	     * checked transactions!
	     */

	    transactionCreator = Thread.currentThread();
	}
	else
	    transactionCreator = null;

	CheckedAction ca = OTS_Current.checkedAction();

	if (ca != null)
	{
	    super.setCheckedAction(ca);
	    ca = null;
	}
    }

public OTS_Transaction (Uid actUid, Control myParent)
    {
	this(actUid, myParent, null);
    }
    
public OTS_Transaction (Uid actUid, Control myParent, OTS_Transaction parent)
    {
	super(actUid);

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_OTS, "OTS_Transaction Begin for < "+get_uid()+" , "+((parent != null) ? parent.get_uid() : Uid.nullUid())+" >");
	}

	parentTransaction = parent;
	controlHandle = null;
	parentHandle = myParent;
	currentStatus = org.omg.CosTransactions.Status.StatusUnknown;

	rootAction = null;
	_synchs = null;
	
	super.Begin(parent);

	/*
	 * Add uid of this action to parent.
	 */

	if (parent != null)
	    parent.addChildAction(this);
    
	currentStatus = determineStatus(this);
	rootAction = this;

	hashCode = get_uid().hashCode();

	if (parent != null)
	{
	    while ((rootAction.parent()) != null)
		rootAction = rootAction.parent();
    
	    topLevelHashCode = rootAction.get_uid().hashCode();
	}
	else
	    topLevelHashCode = hashCode;	

	if (OTS_Transaction._checkedTransactions)
	{
	    /*
	     * Fully checked transactions only allow the thread which began
	     * the transaction to terminate it. We get the id of the beginning
	     * thread here.
	     */

	    transactionCreator = Thread.currentThread();
	}
	else
	    transactionCreator = null;

	CheckedAction ca = OTS_Current.checkedAction();

	if (ca != null)
	{
	    super.setCheckedAction(ca);
	    ca = null;
	}
    }

    /*
     * Memory management is much better in Java, so we don't have the
     * problem of the Control referencing the transaction and vice versa.
     */

public void finalize ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.DESTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_OTS, "OTS_Transaction.finalize - called for < "+get_uid()+" >");
	}
	
	if (_synchs != null)
	{
	    // should not happen if the transaction has terminated
	    
	    ErrorStream.stream(ErrorStream.WARNING).println("OTS_Transaction.finalize() - none zero synchronization list!");

	    // "delete" list anyway, but don't do anything with list elements
	    
	    _synchs = null;
	}

	controlHandle = null;

	super.finalize();
    }

public final synchronized OTS_Control getControlHandle ()
    {
	return controlHandle;
    }

public final synchronized void setControlHandle (OTS_Control handle)
    {
	controlHandle = handle;
    }
 
/**
 * If the transaction has already been committed (by another thread, for
 * example) then we do nothing - could throw TransactionRequired or
 * InvalidTransaction.
 * However, if it was rolledback then we throw TransactionRolledBack. Seems
 * like an inconsistency.
 *
 * report_heuristics is ignored if we are a subtransaction.
 */
 

public synchronized void commit (boolean report_heuristics) throws HeuristicMixed, HeuristicHazard, SystemException

    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_OTS, "OTS_Transaction::commit for "+get_uid());
	}

	if (OTS_Transaction._checkedTransactions && !checkAccess())
	{
	    throw new NO_PERMISSION(0, CompletionStatus.COMPLETED_NO);
	}

	int outcome = super.status();

	if ((outcome == ActionStatus.RUNNING) || (outcome == ActionStatus.ABORT_ONLY)) // have we already been committed?
	{
	    try
	    {
		if (_synchs != null)
		    beforeCompletion();
	    }
	    catch (Exception e)
	    {
		/*
		 * Don't do anything, since we will have marked the
		 * transaction as rollback only.
		 */
	    }
	    
	    /*
	     * Remove the uid of this action from the parent.
	     */

	    if (parentTransaction != null)
	    {
		parentTransaction.removeChildAction(this);
	    }

	    outcome = super.End(report_heuristics);

	    try
	    {
		if (_synchs != null)
		{
		    currentStatus = determineStatus(this);
		    
		    afterCompletion(currentStatus);

		    _synchs = null;
		}
	    }
	    catch (Exception e)
	    {
	    }

	    destroyAction();
	}
	else
	{
	    /*
	     * Differentiate between us committing the transaction
	     * and some other thread doing it.
	     */


	    throw new INVALID_TRANSACTION (0, CompletionStatus.COMPLETED_NO);

	}

	switch (outcome)
	{
	case ActionStatus.COMMITTED:
	case ActionStatus.H_COMMIT:
	case ActionStatus.COMMITTING:  // in case asynchronous commit!
	    return;
	case ActionStatus.ABORTED:
	case ActionStatus.H_ROLLBACK:

	    throw new TRANSACTION_ROLLEDBACK (OTS_Exceptions.FAILED_TO_COMMIT, CompletionStatus.COMPLETED_NO);

	case ActionStatus.H_MIXED:
	    throw new HeuristicMixed();
	case ActionStatus.H_HAZARD:
	default:
	    throw new HeuristicHazard();
	}
    }

public synchronized void rollback () throws SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_OTS, "OTS_Transaction::rollback for "+get_uid());
	}

	if (OTS_Transaction._checkedTransactions && !checkAccess())
	{
	    throw new NO_PERMISSION(0, CompletionStatus.COMPLETED_NO);
	}

	int status = super.status();
	
	if ((status == ActionStatus.RUNNING) ||
	    (status == ActionStatus.ABORT_ONLY)) // already aborted?
	{
	    if (OTS_Transaction._syncOn)
	    {
		try
		{
		    if (_synchs != null)
			beforeCompletion();
		}
		catch (Exception e)
		{
		    /*
		     * Don't do anything - we're about to rollback anyway!
		     */
		}
	    }
	    else
	    {
		/*
		 * If we have any synchronizations delete them now. Can only be
		 * a top-level action.
		 */

		_synchs = null;
	    }

	    /*
	     * Remove uid of this action from parent even if remote.
	     */

	    if (parentTransaction != null)
	    {
		parentTransaction.removeChildAction(this);
	    }

	    super.Abort();

	    if (OTS_Transaction._syncOn)
	    {
		try
		{
		    if (_synchs != null)
		    {
			currentStatus = determineStatus(this);
			
			afterCompletion(currentStatus);
		    }
		}
		catch (Exception e)
		{
		}
	    }

	    destroyAction();

	    status = super.status();
	}
	else
	{
	    /*
	     * Differentiate between us ending the transaction
	     * and some other thread doing it.
	     */


		throw new INVALID_TRANSACTION (0, CompletionStatus.COMPLETED_NO); // means transaction already terminated.

	}

	switch (status)
	{
	case ActionStatus.ABORTING:
	case ActionStatus.ABORTED:
	case ActionStatus.H_ROLLBACK:
	    /*
	     * If the transaction has already rolledback then
	     * silently ignore the multiple rollback attempts.
	     */
	    return;
	case ActionStatus.PREPARING:  // shouldn't be able to get heuristics or any of these!
	case ActionStatus.PREPARED:
	case ActionStatus.COMMITTING:
	case ActionStatus.COMMITTED:
	case ActionStatus.H_COMMIT:
	case ActionStatus.H_MIXED:
	case ActionStatus.H_HAZARD:

	    throw new INVALID_TRANSACTION (0, CompletionStatus.COMPLETED_NO); // means transaction already terminated.

	case ActionStatus.INVALID:
	case ActionStatus.CLEANUP:
	    throw new UNKNOWN(OTS_Exceptions.UNKNOWN_EXCEPTION, CompletionStatus.COMPLETED_MAYBE);
	}
    }

public org.omg.CosTransactions.Status get_status () throws SystemException
    {
	Status s = determineStatus(this);

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_OTS, "OTS_Transaction::get_status for "+get_uid()+" returning "+OTS_Utility.printStatus(s));
	}

	return s;
    }

public org.omg.CosTransactions.Status get_parent_status () throws SystemException
    {
	if (parentTransaction != null)
	    return parentTransaction.get_status();
	else
	    return get_status();
    }

public org.omg.CosTransactions.Status get_top_level_status () throws SystemException
    {
	if (rootAction != null)
	    return determineStatus(rootAction);
	else
	    return get_status();
    }

public boolean is_same_transaction (Coordinator tc) throws SystemException
    {
	if (tc == null)
	    return false;
	
	/*
	 * Cut down the amount of work we need to do. Hash values
	 * for the same transaction must be the same!
	 */

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_OTS, "OTS_Transaction::is_same_transaction comparing hash codes: < "+tc.hash_transaction()+", "+hash_transaction()+" >");
	}

	if (tc.hash_transaction() != hash_transaction())
	    return false;

	boolean result = false;
    
	try
	{
	    UidCoordinator ptr =  com.arjuna.ArjunaOTS.UidCoordinatorHelper.narrow(tc) ;

	    if (ptr != null)
	    {
		/*
		 * Must be an Arjuna coordinator.
		 */

		String myUid = uid();
		String compareUid = ptr.uid();

		if (DebugController.enabled())
		{
		    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
							 FacilityCode.FAC_OTS, "OTS_Transaction::is_same_transaction comparing uids < "+compareUid+", "+myUid+" >");
		}

		if (myUid.compareTo(compareUid) == 0)
		    result = true;

		myUid = null;
		compareUid = null;

		ptr = null;
	    }
	    else
		throw new BAD_PARAM();
	}
	catch (SystemException e)
	{
	    /*
	     * Narrow failed, so can't be an Arjuna Uid. Therefore, the
	     * answer must be false.
	     */
	}
	
	return result;
    }

public boolean is_related_transaction (Coordinator tc) throws SystemException
    {
	if (tc == null)
	    return false;
	
	boolean result = false;
    
	try
	{
	    UidCoordinator ptr =  com.arjuna.ArjunaOTS.UidCoordinatorHelper.narrow(tc) ;

	    if (ptr != null)
	    {
		/*
		 * Must be an Arjuna coordinator.
		 */

		/*
		 * If they have the same parent, then they
		 * must be related.
		 */
		
		String myTLUid = topLevelUid();
		String compareTLUid = ptr.topLevelUid();

		if (DebugController.enabled())
		{
		    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
							 FacilityCode.FAC_OTS, "OTS_Transaction::is_related_transaction comparing uids < "+compareTLUid+", "+myTLUid+" >");
		}

		if (myTLUid.compareTo(compareTLUid) == 0)
		    result = true;

		myTLUid = null;
		compareTLUid = null;

		ptr = null;
	    }
	    else
		throw new BAD_PARAM();
	}
	catch (SystemException e)
	{
	    /*
	     * Narrow failed, so can't be an Arjuna Uid. Therefore, the
	     * answer must be false.
	     */
	}
    
	return result;
    }

/**
 * Is this transaction an ancestor of tc?
 */
 
public boolean is_ancestor_transaction (Coordinator tc) throws SystemException
    {
	if (tc == null)
	    return false;

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_OTS, "OTS_Transaction::is_ancestor_transaction ()");
	}

	if (is_same_transaction(tc))
	    return true;
	else
	{
	    /*
	     * Are we related?
	     */
	    
	    if (is_related_transaction(tc))
	    {
		if (is_descendant_transaction(tc))
		    return false;
		else
		    return true;
	    }
	    else
		return false;
	}
    }

/**
 * Is this transaction a descendant of tc?
 */
 
public boolean is_descendant_transaction (Coordinator tc) throws SystemException
    {
	if (tc == null)
	    return false;

	try
	{
	    UidCoordinator ptr =  com.arjuna.ArjunaOTS.UidCoordinatorHelper.narrow(tc) ;

	    if (ptr != null)
	    {
		/*
		 * Must be an Arjuna coordinator.
		 */

		Uid lookingFor = new Uid(ptr.uid());
		BasicAction lookingAt = this;

		if (DebugController.enabled())
		{
		    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
							 FacilityCode.FAC_OTS, "OTS_Transaction::is_descendant_transaction - looking for "+lookingFor);
		}

		while (lookingAt != null)
		{
		    if (DebugController.enabled())
		    {
			DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
							     FacilityCode.FAC_OTS, "OTS_Transaction::is_descendant_transaction - looking for "+lookingAt.get_uid());
		}

		    if (lookingAt.get_uid().equals(lookingFor))
			return true;
		    else
			lookingAt = lookingAt.parent();
		}

		ptr = null;
	    }
	    else
		throw new BAD_PARAM();
	}
	catch (SystemException e)
	{
	    /*
	     * Narrow failed, so can't be an Arjuna Uid. Therefore, the
	     * answer must be false.
	     */
	}

	return false;
    }

public boolean is_top_level_transaction () throws SystemException
    {
	return (this == rootAction);
    }

public int hash_transaction () throws SystemException
    {
	return hashCode;
    }

public int hash_top_level_tran () throws SystemException
    {
	return topLevelHashCode;
    }

/**
 * Resources are only registered with the current transaction, whereas
 * subtransaction aware resources are registered with their parents when the
 * current transaction ends.
 */

public RecoveryCoordinator register_resource (Resource r) throws SystemException, Inactive
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_OTS, "OTS_Transaction::register_resource ( "+r+" ) - called for "+get_uid());
	}
	
	if (r == null)
	    throw new BAD_PARAM(0, CompletionStatus.COMPLETED_NO);

	currentStatus = determineStatus(this);
	
	if (currentStatus != Status.StatusActive)
	{
	    if (DebugController.enabled())
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						     FacilityCode.FAC_OTS, "OTS_Transaction::register_resource - transaction not active: "+OTS_Utility.printStatus(currentStatus));
	    }

	    if (currentStatus == Status.StatusMarkedRollback)
	    {

		throw new TRANSACTION_ROLLEDBACK (OTS_Exceptions.MARKED_ROLLEDBACK, CompletionStatus.COMPLETED_NO);

	    }
	    else
		throw new Inactive();
	}

	AbstractRecord corbaRec = null;
	BasicAction registerIn = this;

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_OTS, "OTS_Transaction "+get_uid()+" ::register_resource: ");
	}

	//
	// Creation of recovery coordinator (DBI)
	//
	
	//
	// Pack the params:
	// [0] = OTS_Transaction* this
	//

	int index = 0;
	Object params[] = new Object[10];
	params[index++] = this;

	RecoveryCoordinator recoveryCoordinator = null;
	Uid recoveryCoordinatorUid = null;

	try
	{
	    recoveryCoordinator = OTS_RecoveryCreator.createRecoveryCoordinator(r, params);

	    if (recoveryCoordinator == null)
		throw new BAD_OPERATION("RecoveryCoordinator not created!");
	}
	catch (NO_IMPLEMENT ex)
	{
	    /*
	     * This is legal, and is meant to show that this ORB or
	     * configuration simply doesn't support crash recovery.
	     */

	    recoveryCoordinator = null;
	}
	catch (SystemException e)
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("RecoveryCoordinator creation for "+get_uid()+" threw: "+e);
	    
	    /*
	     * Set transaction to rollback only and re-throw exception.
	     */

	    try
	    {
		rollback_only();
	    }
	    catch (Inactive ex1)
	    {
	    }
	    catch (SystemException ex2)
	    {
		ErrorStream.stream(ErrorStream.WARNING).println("Attempt to set rollback_only for "+get_uid()+" threw: "+ex2);

		throw ex2;
	    }
	    
	    throw e;
	}
	
	if (recoveryCoordinator != null)
	{
	    //
	    // We got a RecoveryCoordinator, so unpack the other return values:
	    // [0] = RecoveryCoordinator Uid*
	    //

	    index = 0;
	    recoveryCoordinatorUid = (com.arjuna.ArjunaCommon.Common.Uid) params[index++];
	}
	else
	{
	    //
	    // We didn't get a RecoveryCoordinator, so we don't assume that
	    // the other return values have been populated.
	    //

	    recoveryCoordinatorUid = Uid.nullUid();
	}

	try
	{
	    SubtransactionAwareResource staResource =  org.omg.CosTransactions.SubtransactionAwareResourceHelper.narrow(r) ;
	    
	    /*
	     * Some Orbs (e.g., Orbix) throw BAD_PARAM is the object in
	     * X.narrow is not of type X, whereas others (e.g., OrbPlus)
	     * simply return NULL!
	     */

	    if (staResource != null)
	    {
		if (DebugController.enabled())
		{
		    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
							 FacilityCode.FAC_OTS, "OTS_Transaction::register_resource for "+get_uid()+" - subtransaction aware resource: YES");
		}

		/*
		 * If here the narrow was ok so we have a subtran aware
		 * resource.
		 */

		Coordinator coord = null;

		if (parentHandle != null)
		{
		    /*
		     * If we are a SubTranResource then we get registered with
		     * the current transaction and its parents upon completion.
		     * The first parameter to the record indicates whether we
		     * should be propagated (registered) with the parent
		     * transaction.
		     */

		    coord = parentHandle.get_coordinator();
		}

		corbaRec = createOTSRecord(true, r, coord, recoveryCoordinatorUid);
		
		coord = null;
		staResource = null;
	    }
	    else
		throw new BAD_PARAM(ORBExceptions.BAD_OBJECT_REF, CompletionStatus.COMPLETED_NO);
	}
	catch (BAD_PARAM ex)
	{
	    if (DebugController.enabled())
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						     FacilityCode.FAC_OTS, "OTS_Transaction::register_resource for "+get_uid()+" - subtransaction aware resource: NO");
	    }

	    /* narrow failed must be a plain resource */
	    
	    /*
	     * Register with current transaction, but we only receive
	     * invocations at top-level.
	     */

	    if (DebugController.enabled())
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						     FacilityCode.FAC_OTS, "OTS_Transaction "+get_uid()+" ::register_resource: Simple resource - "+ex);
	    }

	    corbaRec = createOTSRecord(true, r, null, recoveryCoordinatorUid);
	}
	catch (Unavailable e1)
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("OTS_Transaction.register_resource: catch (Unavailable)");

	    throw new Inactive();
	}
	catch (SystemException e2)
	{
	    if (DebugController.enabled())
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						     FacilityCode.FAC_OTS, "OTS_Transaction::register_resource for "+get_uid()+" : catch (SystemException) - "+e2);
	    }

	    ErrorStream.stream(ErrorStream.WARNING).println("OTS_Transaction.register_resource: catch (SystemException) : "+e2);

	    throw new UNKNOWN(OTS_Exceptions.UNEXPECTED_SYSTEMEXCEPTION, CompletionStatus.COMPLETED_NO);
	}
	catch (Exception e3)
	{
	    if (DebugController.enabled())
	    {

		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						     FacilityCode.FAC_OTS, "OTS_Transaction::register_resource for "+get_uid()+" : catch (...) - "+e3);
	    }

	    /*
	     * Cannot just rethrow exception, so throw UNKNOWN.
	     */
	    
	    ErrorStream.stream(ErrorStream.WARNING).println("OTS_Transaction.register_resource: catch (...) : "+e3);
	    
	    throw new UNKNOWN(OTS_Exceptions.UNKNOWN_EXCEPTION, CompletionStatus.COMPLETED_NO);
	}

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_OTS, "OTS_Transaction::register_resource for "+get_uid()+" : try end");
	}

	if (registerIn.add(corbaRec) != AddOutcome.AR_ADDED)
	{
	    corbaRec = null;


	    throw new INVALID_TRANSACTION (OTS_Exceptions.ADD_FAILED, CompletionStatus.COMPLETED_NO);

	}
	else
	{
	    if (DebugController.enabled())
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						     FacilityCode.FAC_OTS, "OTS_Transaction::register_resource for "+get_uid()+" : resource registered");
	    }
	}
	
	return recoveryCoordinator;
    }

/**
 * Do not propagate the resource to the parent.
 */

public void register_subtran_aware (SubtransactionAwareResource r) throws Inactive, NotSubtransaction, SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_OTS, "OTS_Transaction::register_subtran_aware called for "+get_uid());
	}

	if (r == null)
	    throw new BAD_PARAM(ORBExceptions.BAD_OBJECT_REF, CompletionStatus.COMPLETED_NO);

	currentStatus = determineStatus(this);
	
	if (currentStatus != Status.StatusActive)
	{
	    if (currentStatus == Status.StatusMarkedRollback)
	    {

		throw new TRANSACTION_ROLLEDBACK (OTS_Exceptions.MARKED_ROLLEDBACK, CompletionStatus.COMPLETED_NO);

	    }
	    else
		throw new Inactive();
	}	
	
	if (this == rootAction)
	{
	    if (DebugController.enabled())
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						     FacilityCode.FAC_OTS, "OTS_Transaction::register_subtran_aware called for "+get_uid()+" : not a subtransaction!");
	    }

	    throw new NotSubtransaction();
	}
	else
	{
	    Coordinator coord = null;
	    AbstractRecord corbaRec = null;
	    
	    try
	    {
		coord = parentHandle.get_coordinator();
		corbaRec = createOTSRecord(false, r, coord);
	    }
	    catch (Unavailable ex)
	    {
		throw new UNKNOWN(OTS_Exceptions.INACTIVE_TRANSACTION, CompletionStatus.COMPLETED_NO);  // what else to raise?
	    }

	    coord = null;
	
	    /*
	     * Throw some exception here?
	     */
	
	    if (add(corbaRec) != AddOutcome.AR_ADDED)
	    {
		if (DebugController.enabled())
		{
		    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
							 FacilityCode.FAC_OTS, "OTS_Transaction::register_subtran_aware called for "+get_uid()+" : could not add.");
		}

		corbaRec = null;
		throw new Inactive();  // what else to raise??
	    }
	}

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_OTS, "OTS_Transaction::register_subtran_aware called for "+get_uid()+" : subtran_aware_resource registered");
	}
    }

public void rollback_only () throws SystemException, Inactive
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_OTS, "OTS_Transaction::rollback_only - called for "+get_uid());
	}

	if (determineStatus(this) == Status.StatusActive)
	{
	    if (!preventCommit())
	    {

		throw new INVALID_TRANSACTION (OTS_Exceptions.INACTIVE_TRANSACTION, CompletionStatus.COMPLETED_NO);

	    }
	}
	else
	    throw new Inactive();	
    }

/**
 * To be used for debugging purposes only.
 */

public String get_transaction_name () throws SystemException
    {
	return get_uid().stringForm();
    }

public Control create_subtransaction () throws SystemException, SubtransactionsUnavailable, Inactive
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_OTS, "OTS_Transaction::create_subtransaction - called for "+get_uid());
	}

	if (determineStatus(this) != Status.StatusActive)
	    throw new Inactive();
	else
	{
	    if (!_subtran)
		throw new SubtransactionsUnavailable();
	    else
	    {
		if (controlHandle == null)
		    throw new Inactive();
		else
		    return OTS_Factory.create_subtransaction(controlHandle.getControl(), this);
	    }
	}
    }


 
/**
 * The spec states that a synchronization is registered with a single
 * top-level action only. However, if this is a nested transaction there
 * is no appropriate exception to raise. So, we raise
 * SynchronizationUnavailable. We could simply get our parent and register
 * the synchronization with it, but this may not be what the user expects.
 * If it is, then the user can get the parent and do it directly!
 */

public synchronized void register_synchronization (Synchronization sync) throws Inactive, SynchronizationUnavailable, SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_OTS, "OTS_Transaction::register_synchronization - called for "+get_uid());
	}

	if (sync == null)
	    throw new BAD_PARAM(ORBExceptions.BAD_OBJECT_REF, CompletionStatus.COMPLETED_NO);
	
	if (!is_top_level_transaction())  // are we a top-level transaction?
	{
	    if (DebugController.enabled())
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						     FacilityCode.FAC_OTS, "OTS_Transaction::register_synchronization - "+get_uid()+" is not a top-level transaction!");
	    }
	    
	    throw new SynchronizationUnavailable();
	}
	else
	{
	    currentStatus = determineStatus(this);
	    
	    if (currentStatus == Status.StatusActive)  // is transaction still running?
	    {
		if (_synchs == null)
		    _synchs = new HashList(10);

		OTS_Synchronization otsSync = new OTS_Synchronization(sync);
	    
		if (!_synchs.add(otsSync))
		{
		    otsSync = null;
		    throw new UNKNOWN(OTS_Exceptions.ADD_FAILED, CompletionStatus.COMPLETED_NO);  // what else to raise?
		}
	    }
	    else
	    {
		if (DebugController.enabled())
		{
		    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
							 FacilityCode.FAC_OTS, "OTS_Transaction::register_synchronization - "+get_uid()+" is not active: "+OTS_Utility.printStatus(currentStatus));
		}

		if (currentStatus == Status.StatusMarkedRollback)
		{

		    throw new TRANSACTION_ROLLEDBACK (OTS_Exceptions.MARKED_ROLLEDBACK, CompletionStatus.COMPLETED_NO);

		}
		else
		    throw new Inactive();
	    }
	}
    }






 
public synchronized PropagationContext get_txcontext () throws Unavailable, SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_OTS, "OTS_Transaction::get_txcontext - called for "+get_uid());
	}

	/*
	 * Throw an exception if we are not active.
	 */
    
	currentStatus = determineStatus(this);
	
	if ((currentStatus != Status.StatusActive) &&
	    (currentStatus != Status.StatusMarkedRollback))
	{
	    throw new Unavailable();
	}
	else
	{
	    try
	    {
		return propagationContext();
	    }
	    catch (Exception e)
	    {
		throw new Unavailable();
	    }
	}
    }
 


/*
 * Some Arjuna specific methods.
 */

/**
 * We use these to determine relationships between transactions. Using the hash
 * function is not sufficient, since a hash value is not guaranteed to be
 * unique.
 */
  
public String uid () throws SystemException
    {
	return get_uid().stringForm();
    }

public String topLevelUid () throws SystemException
    {
	if (rootAction != null)
	    return rootAction.get_uid().stringForm();
	else
	    return null;
    }

public String type ()
    {
	return OTS_Transaction.typeName();
    }

public static final int interpositionType ()
    {
	return _ipType;
    }

public static String typeName ()
    {
	return "/StateManager/BasicAction/OTS_Transaction";
    }

public String toString ()
    {
	return "OTS_Transaction < "+get_uid()+" >";
    }

public boolean equals (Object o)
    {
	if (o instanceof OTS_Transaction)
	{
	    OTS_Transaction tx = (OTS_Transaction) o;
	    
	    if (tx == this)
		return true;
	    else
		return tx.get_uid().equals(get_uid());
	}
	else
	    return false;
    }

public boolean forgetHeuristics ()
    {
	return super.forgetHeuristics();
    }
	    
/**
 * For crash recovery purposes only.
 */
 
protected OTS_Transaction (Uid actUid)
    {
	super(actUid);

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PROTECTED,
						 FacilityCode.FAC_OTS, "OTS_Transaction::OTS_Transaction ( "+actUid+" )");
	}

	parentTransaction = null;
	controlHandle = null;
	parentHandle = null;
	currentStatus = org.omg.CosTransactions.Status.StatusUnknown;

	transactionCreator = null;

	rootAction = null;
	_synchs = null;
	
	/*
	 * Leave activation of transaction up to caller. Transaction status
	 * will remain as Unknown until then.
	 */	
	
	rootAction = this;

	/*
	 * Do this once to avoid overhead.
	 */
    
	hashCode = actUid.hashCode();
	topLevelHashCode = hashCode;  // this must be a top-level transaction

	/*
	 * Don't bother with checked transactions for recovery.
	 */
    }
 
protected void beforeCompletion () throws SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PROTECTED,
						 FacilityCode.FAC_OTS, "OTS_Transaction::beforeCompletion for "+get_uid());
	}

	boolean problem = false;
	
	/*
	 * If we have a synchronization list then we must be top-level.
	 */

	if (_synchs != null)
	{
	    HashListIterator iterator = new HashListIterator(_synchs);
	    OTS_Synchronization value = (OTS_Synchronization) iterator.iterate();

	    try
	    {
		while (value != null)
		{
		    Synchronization c = value.contents();

		    c.before_completion();
		    
		    value = (OTS_Synchronization) iterator.iterate();
		}
	    }
	    catch (SystemException e)
	    {
		ErrorStream.stream(ErrorStream.WARNING).println("OTS_Transaction.beforeCompletion - caught exception.");
		
		if (!problem)
		{
		    problem = true;
		    
		    /*
		     * Mark as rollback_only, so when we try to commit it
		     * will fail.
		     */

		    try
		    {
			rollback_only();
		    }
		    catch (Inactive ex)
		    {
			/*
			 * This should not happen. If it does, continue with
			 * commit to tidy-up.
			 */
		
			ErrorStream.stream(ErrorStream.WARNING).println("OTS_Transaction.beforeCompletion - attempted rollback_only failed!");
		    }
		}
	    }
	}

	if (problem)
	    throw new UNKNOWN(OTS_Exceptions.SYNCHRONIZATION_EXCEPTION, CompletionStatus.COMPLETED_NO);
    }

protected void afterCompletion (org.omg.CosTransactions.Status myStatus) throws SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PROTECTED,
						 FacilityCode.FAC_OTS, "OTS_Transaction::afterCompletion for "+get_uid());
	}
	
	if (myStatus == Status.StatusActive)
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("OTS_Transaction.afterCompletion called on still running transaction!");
	    return;
	}

	boolean problem = true;
	
	if (_synchs != null)
	{
	    HashListIterator iterator = new HashListIterator(_synchs);
	    OTS_Synchronization value = (OTS_Synchronization) iterator.iterate();
		
	    /*
	     * Regardless of failures, we must tell all synchronizations
	     * what happened.
	     */
	
	    while (value != null)
	    {
		Synchronization c = value.contents();
		    
		try
		{
		    c.after_completion(myStatus);
		}
		catch (SystemException e)
		{
		    if (DebugController.enabled())
		    {
			ErrorStream.stream(ErrorStream.WARNING).println("OTS_Transaction.afterCompletion - caught exception "+e);
		    }

		    problem = true;
		}

		value = (OTS_Synchronization) iterator.iterate();
	    }

	    _synchs = null;
	}

	if (problem)
	    throw new UNKNOWN(OTS_Exceptions.SYNCHRONIZATION_EXCEPTION, CompletionStatus.COMPLETED_NO);
    }

/**
 * Called by transaction reaper to force rollback. We do not check
 * the id of the calling thread here, as it will definitely not be
 * the thread which created this transaction. With checked transactions
 * this is normally not allowed, so we need some way to circumvent this.
 */

final synchronized void forceRollback () throws SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_OTS, "OTS_Transaction::forceRollback for "+get_uid());
	}

	if (super.status() == ActionStatus.RUNNING)  // already aborted?
	{
	    /*
	     * If we have any synchronizations delete them now. Can only be
	     * a top-level action.
	     */
    
	    if (_synchs != null)
	    {
		if (OTS_Transaction._syncOn)
		{
		    try
		    {
			if (_synchs != null)
			    beforeCompletion();
		    }
		    catch (Exception e)
		    {
		    }
		}
		
		_synchs = null;
	    }

	    /*
	     * Remove uid of this action from parent even if remote.
	     */

	    if (parentTransaction != null)
	    {
		parentTransaction.removeChildAction(this);
	    }

	    Abort();

	    if (OTS_Transaction._syncOn)
	    {
		try
		{
		    if (_synchs != null)
		    {
			currentStatus = determineStatus(this);
			
			afterCompletion(currentStatus);
		    }
		}
		catch (Exception e)
		{
		}
	    }

	    destroyAction();
	}
    }

static boolean getTransactions (InputObjectState os)
    {
	return getTransactions(os, ObjectStore.OS_UNKNOWN);
    }
 
static boolean getTransactions (InputObjectState os, int status)
    {
	ObjectStore store = new ObjectStore(com.arjuna.ArjunaCore.Atomic.InitAction.actionStoreType);

	try
	{
	    return store.allObjUids(typeName(), os, status);
	}
	catch (NullPointerException ex)
	{
	}
	catch (ObjectStoreException e)
	{
	    e.printStackTrace();
	}

	return false;
    }

private final org.omg.CosTransactions.Status determineStatus (BasicAction whichAction)
    {
	org.omg.CosTransactions.Status theStatus = org.omg.CosTransactions.Status.StatusUnknown;
    
	if (whichAction != null)
	{
	    switch (whichAction.status())
	    {
	    case ActionStatus.INVALID:  // probably locked, so try again later
		theStatus = Status.StatusUnknown;
		break;
	    case ActionStatus.RUNNING:
		theStatus = Status.StatusActive;
		break;
	    case ActionStatus.PREPARED:
		theStatus = Status.StatusPrepared;
		break;
	    case ActionStatus.COMMITTED:
	    case ActionStatus.H_COMMIT:
	    case ActionStatus.H_MIXED:
	    case ActionStatus.H_HAZARD:
		theStatus = Status.StatusCommitted;
		break;
	    case ActionStatus.ABORTED:
	    case ActionStatus.H_ROLLBACK:
		theStatus = Status.StatusRolledBack;
		break;
	    case ActionStatus.ABORT_ONLY:
		theStatus = Status.StatusMarkedRollback;
		break;

	    case ActionStatus.PREPARING:
		theStatus = Status.StatusPreparing;
		break;
	    case ActionStatus.COMMITTING:
		theStatus = Status.StatusCommitting;
		break;
	    case ActionStatus.ABORTING:
		theStatus = Status.StatusRollingBack;
		break;

	    default:
		theStatus = Status.StatusUnknown;
	    }
	}
	else
	    theStatus = Status.StatusNoTransaction;

	return theStatus;
    }

protected final AbstractRecord createOTSRecord (boolean propagate, Resource resource,
						Coordinator coord)
    {
	return createOTSRecord(propagate, resource, coord, null);
    }
    
protected final AbstractRecord createOTSRecord (boolean propagate, Resource resource,
						Coordinator coord, Uid recCoordUid)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PRIVATE,
						 FacilityCode.FAC_OTS, "OTS_Transaction::createOTSRecord for "+get_uid());
	}

	/*
	 * If the resource is an ArjunaOTS.OTSAbstractRecord or an
	 * ArjunaOTS.ArjunaSubtranAwareResource then we can do better
	 * record manipulation, and proper nested actions.
	 *
	 * Based on the type of resource we create the right abstract record
	 * to handle it, rather than a single abstract record which switches
	 * protocols internally.
	 */

	ArjunaSubtranAwareResource absRec = null;
	AbstractRecord corbaRec = null;

	if (resource != null)
	{
	    try
	    {
		absRec =  com.arjuna.ArjunaOTS.ArjunaSubtranAwareResourceHelper.narrow(resource) ;

		if (absRec == null)
		    throw new BAD_PARAM(ORBExceptions.BAD_OBJECT_REF, CompletionStatus.COMPLETED_NO);
	    }
	    catch (Exception e)
	    {
		// can't be an ArjunaOTS.ArjunaSubtranAwareResource

		absRec = null;
	    }
	}

	if (absRec == null)
	{
	    corbaRec = new OTS_ResourceRecord(propagate, resource, coord, recCoordUid, this);
	}
	else
	{
	    Uid u = null;
	    OTSAbstractRecord otsRec;

	    try
	    {	
		otsRec =  com.arjuna.ArjunaOTS.OTSAbstractRecordHelper.narrow(absRec) ;
		
		if (otsRec == null)
		    throw new BAD_PARAM(ORBExceptions.BAD_OBJECT_REF, CompletionStatus.COMPLETED_NO);
	    }
	    catch (Exception e)
	    {
		otsRec = null;
	    }

	    if (otsRec != null)
	    {
		try
		{
		    u = new Uid(otsRec.uid());
		}
		catch (Exception e)
		{
		    u = null;
		}

		if (u == null)
		    ErrorStream.stream(ErrorStream.WARNING).println("OTS_Transaction.createOTSRecord - OTS_AbstractRecord could not get uid of object.");
	    }

	    if (u == null)
		u = new Uid();
		
	    corbaRec = new OTS_AbstractRecord(propagate, u, absRec, coord, recCoordUid, this);

	    otsRec = null;
	    absRec = null;
	    u = null;
	}

	return corbaRec;
    }
 
/*
 * Is the calling thread the one which began this transaction?
 */

private final boolean checkAccess ()
    {
	if (Thread.currentThread() == transactionCreator)
	    return true;
	else
	    return false;
    }

/*
 * The caller should delete the context.
 *
 * The propagation context is specified on a per client thread basis.
 * Therefore, at the server side we must maintain a hierarchy for each
 * thread. However, the server cannot simply tear down this hierarchy
 * whenever it receives a completely new one from the same thread, since
 * the OTS lets a thread suspend/resume contexts at will. Potential for
 * memory leaks in C++ version, but not Java!!
 *
 * Currently we assume that the hierarchy will be Arjuna transactions so
 * we can get the parents of transactions. If it is not then we could
 * simply just call get_txcontext on the control!
 */

private final synchronized PropagationContext propagationContext () throws Unavailable, Inactive, SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PRIVATE,
						 FacilityCode.FAC_OTS, "OTS_Transaction::propagationContext for "+get_uid());
	}

	String theUid = null;
	Control currentControl = controlHandle.getControl();
	PropagationContext context = new PropagationContext();
	UidCoordinator uidCoord = null;
	int sequenceThreshold = 5;
	int sequenceIncrement = 5;

	context.parents = null;
	
	context.currentTransaction = new TransIdentity();
	context.implementation_specific_data = ORBInterface.orb().create_any(); // uughh!!



	/*
	 * Set up current information. We must leave the timeout
	 * for now, since it only applies to the top-level transaction.
	 */

	try
	{
	    context.currentTransaction.coord = controlHandle.get_coordinator();
	    context.timeout = 0; // will reset later!
	
	    /*
	     * Only send the terminator if explicitly asked to.
	     * This prevents anyone other than the creator from terminating
	     * a transaction.
	     */

	    if (OTS_Transaction._propagateTerminator)
		context.currentTransaction.term = controlHandle.get_terminator();
	    else
		context.currentTransaction.term = null;
	}
	catch (Exception e)
	{
	    return null;
	}

	/*
	 * We send the Uid hierarchy as the otid_t part of
	 * the TransIdentity.
	 */

	try
	{
	    uidCoord = OTS_Helper.getUidCoordinator(context.currentTransaction.coord);

	    if (uidCoord == null)
		throw new BAD_PARAM(ORBExceptions.BAD_OBJECT_REF, CompletionStatus.COMPLETED_NO);
	}
	catch (BAD_PARAM e)
	{
	    return null;
	}
	catch (Exception e)
	{
	    return null;
	}

	// the sequence should do the memory management for us.
	
	theUid = uidCoord.uid();

	context.currentTransaction.otid = OTS_Utility.uidToOtid(theUid);
	context.currentTransaction.otid.formatID = OTS_Transaction._ipType;
	
	theUid = null;
	uidCoord = null;

	int index = 0;
    
	while (currentControl != null)
	{
	    try
	    {
		ActionControl control =  com.arjuna.ArjunaOTS.ActionControlHelper.narrow(currentControl) ;

		if (control != null)
		{
		    /*
		     * Must be an Arjuna control.
		     */

		    currentControl = control.getParentControl();

		    if (currentControl != null)
		    {
			if (index == 0) // first time
			{
			    context.parents = new TransIdentity[sequenceThreshold];  // initial length to avoid realloc

			    for (int ii = 0; ii < sequenceThreshold; ii++)
				context.parents[ii] = null;
			}

			context.parents[index] = new TransIdentity();
			context.parents[index].coord = currentControl.get_coordinator();
			if (OTS_Transaction._propagateTerminator)
			    context.parents[index].term = currentControl.get_terminator();
			else
			    context.parents[index].term = null;

			context.parents[index].otid = new otid_t();
			context.parents[index].otid.formatID = OTS_Transaction._ipType;

			/*
			 * Don't bother checking whether narrow works because
			 * we can't cope with mixed transaction types anyway!
			 * If we got here then the root transaction must be an
			 * Arjuna transaction, so the nested transactions
			 * *must* also be Arjuna transactions!
			 */

			uidCoord = OTS_Helper.getUidCoordinator(context.parents[index].coord);

			theUid = uidCoord.uid();
			context.parents[index].otid = OTS_Utility.uidToOtid(theUid);

			theUid = null;
			uidCoord = null;
		    
			index++;

			if (index >= sequenceThreshold)
			{
			    sequenceThreshold = index + sequenceIncrement;
			    context.parents = resizeHierarchy(context.parents, index + sequenceIncrement);
			}
		    }
		    else
		    {
			/*
			 * Found the top-level transaction, so we can now
			 * setup the timeout value. All transactions will
			 * non-zero timeouts will have been registered with
			 * the transaction reaper.
			 */

			/*
			 * Currently send over the original timeout, and
			 * not what is remaining. Will make this configurable
			 * until the specification is clarified.
			 */

			if (OTS_TransactionReaper.transactionReaper() != null)
			    context.timeout = OTS_TransactionReaper.transactionReaper().getTimeout(control);
			else
			    context.timeout = 0;
		    }

		    control = null;
		}
		else
		    throw new BAD_PARAM(ORBExceptions.BAD_OBJECT_REF, CompletionStatus.COMPLETED_NO);
	    }
	    catch (SystemException e)
	    {
		/*
		 * Not an Arjuna control!! Should not happen!!
		 */

		currentControl = null;
	    }
	    catch (Exception e)
	    {
		currentControl = null;
	    }
	}

	try
	{
	    context.parents = resizeHierarchy(context.parents, index);
	}
	catch (Exception e)
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("OTS_Transaction.resizeHierarchy - caught exception: "+e);

	    context = null;
	}

	return context;
    }

    /*
     * Watch out - we can resize down as well as up!
     */
    
private final TransIdentity[] resizeHierarchy (TransIdentity[] current, int size)
    {
	if ((current == null) || (size == 0))
	    return new TransIdentity[0];

	TransIdentity[] toReturn = new TransIdentity[size];
	
	int currentSize = current.length;
	int copySize = ((size < currentSize) ? size : currentSize);

	for (int i = 0; i < copySize; i++)
	{
	    toReturn[i] = current[i];
	    current[i] = null;
	}

	if (copySize < size)
	{
	    for (int j = copySize; j < size; j++)
		toReturn[j] = null;
	}

	return toReturn;
    }

/*
 * Could perhaps do garbage collection here after a certain number
 * of transactions have been destroyed. Environment variable enabled?
 */
 
protected final void destroyAction ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PROTECTED,
						 FacilityCode.FAC_OTS, "OTS_Transaction::destroyAction for "+get_uid());
	}

	/*
	 * Only do if we are a top-level transaction, since otherwise
	 * we cannot have created recovery coordinators.
	 */

	if (parentHandle == null)
	{
	    Object params[] = new Object[1];
	    
	    params[0] = this;
	    
	    OTS_RecoveryCreator.destroyAllRecoveryCoordinators(params);
	}
	else
	    parentHandle = null;

	switch (super.status())
	{
	case ActionStatus.H_ROLLBACK:
	case ActionStatus.H_MIXED:
	case ActionStatus.H_HAZARD:
	    {
		if (BasicAction.maintainHeuristics())
		    return;
	    }
	}

	try
	{
	    /*
	     * We do not need to do worry about deleting the transaction
	     * in Java as we do in C++ because of the
	     * way garbage collection works - the committing thread
	     * has a reference to the transaction which keeps it alive.
	     */

	    if (controlHandle != null)
	    {
		OTS.destroyControl(controlHandle);

		controlHandle = null;
	    }
	}
	catch (ActiveThreads ex1)
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("OTS_Transaction.destroyAction - caught ActiveThreads exception.");
	}
	catch (BadControl ex2)
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("OTS_Transaction.destroyAction - caught BadControl exception.");
	}
	catch (ActiveTransaction ex3)
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("OTS_Transaction.destroyAction - caught ActiveTransaction exception.");
	}
	catch (Destroyed ex4)
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("OTS_Transaction.destroyAction - caught Destroyed exception.");
	}
	catch (OutOfMemoryError ex5)
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("OTS_Transaction.destroyAction - caught OutOfMemoryError.");

	    /*
	     * Rather than try again after running gc simply return and
	     * let the user deal with it. May help with memory!
	     */
            
 	    System.gc();
	}
	catch (Exception ex6)
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("OTS_Transaction.destroyAction - caught unexpected exception: "+ex6);
	}
    }

protected OTS_Transaction parentTransaction; // rather than rely on
                                             // BasicAction.parent()
protected OTS_Control     controlHandle;
 
private int                            hashCode;
private int                            topLevelHashCode;
private Control                        parentHandle;
private org.omg.CosTransactions.Status currentStatus;
private Thread                         transactionCreator;  // probably null most of the time!
 
private BasicAction rootAction;
private HashList    _synchs;

static int     _ipType = Arjuna.XID();
static boolean _subtran = true;
static boolean _syncOn = true;
static boolean _checkedTransactions = false;
static boolean _propagateTerminator = false;
    
static
    {
	String interpositionType = PropertyManager.getProperty(OTS_Environment.OTS_INTERPOSITION);

	if (interpositionType != null)
	{
	    int ipType = Arjuna.nameToXID(interpositionType);

	    if (ipType != -1)
		OTS_Transaction._ipType = ipType;
	    else
		ErrorStream.stream(ErrorStream.WARNING).println("OTS_Transaction.init - unknown interposition type "+interpositionType);
	}

	String supportSubtran = PropertyManager.getProperty(OTS_Environment.OTS_SUPPORT_SUBTRANSACTIONS);

	if (supportSubtran != null)
	{
	    if (supportSubtran.compareTo("NO") == 0)
		_subtran = false;
	}

	String syncOn = PropertyManager.getProperty(OTS_Environment.OTS_SUPPORT_ROLLBACK_SYNC);

	if (syncOn != null)
	{
	    if (syncOn.compareTo("NO") == 0)
		_syncOn = false;
	}

	String checked = PropertyManager.getProperty(OTS_Environment.OTS_CHECKED_TRANSACTIONS);

	if (checked != null)
	{
	    if (checked.compareTo("YES") == 0)
		_checkedTransactions = true;
	}

	String propTerm = PropertyManager.getProperty(OTS_Environment.OTS_PROPAGATE_TERMINATOR);

	if (propTerm != null)
	{
	    if (propTerm.compareTo("YES") == 0)
		_propagateTerminator = true;
	}
    }
 
}
