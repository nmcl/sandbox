/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_ServerTransaction.javatmpl,v 1.7.8.2.2.1.6.8.6.1.2.2.2.2.2.1.22.1 2001/05/04 10:32:09 nmcl Exp $
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
































































package com.arjuna.CosTransactions.Interposition;

import com.arjuna.CosTransactions.OTS_Transaction;
import com.arjuna.CosTransactions.OTS_Exceptions;
import com.arjuna.CosTransactions.OTS_Environment;
import com.arjuna.CosTransactions.OTS_Utility;
import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaCore.Atomic.*;
import com.arjuna.OrbCommon.ORBInterface;
import org.omg.CosTransactions.*;
import org.omg.CORBA.CompletionStatus;

import org.omg.CORBA.SystemException;
import org.omg.CORBA.BAD_OPERATION;
import java.io.IOException;

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
 * This looks like an OTS_Transaction, but is only create for
 * interposition purposes. The classes OTS_ServerTopLevelAction
 * and OTS_ServerNestedAction use instances of this class
 * to drive the server-side protocol.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: OTS_ServerTransaction.javatmpl,v 1.7.8.2.2.1.6.8.6.1.2.2.2.2.2.1.22.1 2001/05/04 10:32:09 nmcl Exp $
 * @since JTS 1.0.
 */

public class OTS_ServerTransaction extends OTS_Transaction
{

public OTS_ServerTransaction (Uid actUid, Control myParent)
    {
	this(actUid, myParent, null);
    }
    
public OTS_ServerTransaction (Uid actUid, Control myParent, OTS_Transaction parentImpl)
    {
	super(actUid, myParent, parentImpl);

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "OTS_ServerTransaction::OTS_ServerTransaction ( "+actUid+", Control myParent, "+((parentImpl != null) ? parentImpl.get_uid() : Uid.nullUid())+" )");
	}

	_savingUid = new Uid();
	_sync = null;
	_beforeCompleted = false;
	_recoveryCoordinator = null;
	_prepState = ActionStatus.COMMITTING;
    }

public void finalize ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.DESTRUCTORS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "OTS_ServerTransaction.finalize ( "+get_uid()+" )");
	}

	_savingUid = null;

	if (_sync != null)
	{
	    _sync.destroy();
	    _sync = null;
	}

	/*
	 * Remember to destroy the recovery coordinator.
	 */

	_recoveryCoordinator = null;
	
	super.finalize();
    }

public String type ()
    {
	return typeName();
    }

public Uid getSavingUid ()
    {
	return _savingUid;
    }
    
public final synchronized int doPrepare ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "OTS_ServerTransaction::doPrepare ( "+get_uid()+" )");
	}

	/*
	 * If transaction has already been prepared then return error status.
	 */

	if (get_status() != org.omg.CosTransactions.Status.StatusActive)
	    return PrepareOutcome.INVALID_TRANSACTION;
	
	_prepState = ActionStatus.PREPARED;

	if (!_beforeCompleted && (_sync != null))
	{
	    /*
	     * Synchronizations should have been called by now if we have them!
	     */

	    ErrorStream.stream(ErrorStream.WARNING).println("OTS_ServerTransaction.doPrepare - synchronizations not called!");

	    /*
	     * Prevent commit!
	     */

	    super.preventCommit();
	}

	/*
	 * If we do not have an interposed synchronization then
	 * before_completions will not have been called yet. So, do it now.
	 */
    
	if (!_interposedSynch)
	{
	    try
	    {
		doBeforeCompletion();
	    }
	    catch (Exception e)
	    {
		/*
		 * Transaction will have been put into a state
		 * which forces it to rollback, so do nothing here.
		 */
	    }
	}
    
	int res = super.prepare(true);

	/*
	 * If read-only, the coordinator will not talk
	 * to us again, so do commit now and destroy the
	 * transaction.
	 *
	 * Otherwise, the transaction is destroyed when the
	 * commit/abort/forget protocol completes.
	 */

	if (res == PrepareOutcome.PREP_READONLY)
	{
	    doPhase2Commit(true);
	}

	return res;
    }

public final synchronized void doForget ()
    {
	super.destroyAction();
    }
    
public final synchronized int doPhase2Commit (boolean readOnly)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "OTS_ServerTransaction::doPhase2Commit ( "+get_uid()+" )");
	}

	/*
	 * If the transaction has already terminated, then return the status.
	 */

	org.omg.CosTransactions.Status s = get_status();

	if (s != org.omg.CosTransactions.Status.StatusPrepared)
	{
	    ErrorStream.warning().println("OTS_ServerTransaction.doPhase2Commit - transaction not prepared: "+OTS_Utility.printStatus(s));
	    
	    return returnStatus();
	}

	if (!readOnly)
	    super.phase2Commit(true);

	/*
	 * Now do after completion stuff.
	 */

	try
	{
	    doAfterCompletion(get_status());
	}
	catch (Exception e)
	{
	}
    
	if (parentTransaction != null)
	    parentTransaction.removeChildAction(this);

	super.destroyAction();

	if (BasicAction.allActions != null)
	    BasicAction.allActions.remove(get_uid());

	return returnStatus();
    }

public final synchronized int doPhase2Abort ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "OTS_ServerTransaction::doPhase2Abort ( "+get_uid()+" )");
	}

	/*
	 * If the transaction has already terminated, then return the status.
	 */

	org.omg.CosTransactions.Status s = get_status();
	
	if ((s == org.omg.CosTransactions.Status.StatusCommitted) ||
	    (s == org.omg.CosTransactions.Status.StatusRolledBack))
	{
	    return returnStatus();
	}

	super.phase2Abort(true);

	/*
	 * Now do after completion stuff.
	 */

	try
	{
	    doAfterCompletion(get_status());
	}
	catch (Exception e)
	{
	}
	
	if (parentTransaction != null)
	{
	    parentTransaction.removeChildAction(this);
	}

	super.destroyAction();

	if (BasicAction.allActions != null)
	    BasicAction.allActions.remove(get_uid());
	
	return returnStatus();
    }

    /*
     * Called for one-phase commit.
     */



public synchronized void doCommit (boolean report_heuristics) throws HeuristicHazard, SystemException


    {
	int outcome = super.status();

	if ((outcome == ActionStatus.RUNNING) || (outcome == ActionStatus.ABORT_ONLY)) // have we already been committed?
	{
	    if (!_interposedSynch)
	    {
		try
		{
		    doBeforeCompletion();
		}
		catch (Exception e)
		{
		    /*
		     * Transaction will have been put into a state
		     * which forces it to rollback, so do nothing here.
		     */
		}
	    }
	    
	    outcome = super.End(report_heuristics);
	}
	else
	{
	    /*
	     * Differentiate between us committing the transaction
	     * and some other thread doing it.
	     */

	    if (parentTransaction != null)
		parentTransaction.removeChildAction(this);


	    throw new INVALID_TRANSACTION (OTS_Exceptions.INVALID_ACTION, CompletionStatus.COMPLETED_NO);

	}

	/*
	 * Now do after completion stuff.
	 */

	try
	{
	    doAfterCompletion(get_status());
	}
	catch (Exception e)
	{
	}

	if (parentTransaction != null)
	    parentTransaction.removeChildAction(this);

	super.destroyAction();

	switch (outcome)
	{
	case ActionStatus.COMMITTED:
	case ActionStatus.H_COMMIT:
	case ActionStatus.COMMITTING:  // in case asynchronous commit!
	    return;
	case ActionStatus.ABORTED:
	case ActionStatus.H_ROLLBACK:


	    throw new TRANSACTION_ROLLEDBACK (OTS_Exceptions.FAILED_TO_COMMIT, CompletionStatus.COMPLETED_NO);



	case ActionStatus.H_HAZARD:
	default:
	    throw new HeuristicHazard();
	}	
    }
    
    /**
     * Registering a synchronization with interposition is a bit complicated!
     * Synchronizations must be called prior to prepare; if no
     * interposed-synchronization is used then either synchronizations would
     * be registered locally (and then ignored by the commit protocol) or
     * they would need to be registered remotely, which would mean a
     * cross-address space call for each synchronization!
     * 
     * The first time a synchronization is registered locally, we register a
     * proxy back with the real coordinator. When that transaction commits, it
     * will call this proxy, which will then drive the locally registered
     * synchronizations (actually it calls appropriate
     * on the transaction to do this.)
     *
     * However, one-phase commit complicates matters even more since we call
     * commit on the interposed coordinator, which runs through the commit and
     * then the after_completion code before returning to the real
     * coordinator's commit call. Rather than separate commit and
     * synchronization code completely from the transaction (in which case we
     * could just call the commit portion here) we let after_completion get
     * called before returning the commit response,
     * and simply ignore the real coordinator's subsequent call to
     * after_completion.
     */
    

    
public synchronized void register_synchronization (Synchronization theSync) throws Inactive, SynchronizationUnavailable, SystemException
    {
	if (!is_top_level_transaction())  // are we a top-level transaction?
	{
	    throw new SynchronizationUnavailable();
	}
	else
	{
	    /*
	     * If we support interposed synchronizations then add one now,
	     * otherwise just add all synchronizations locally.
	     */
	
	    if (_interposedSynch)
	    {
		if (_sync == null)
		{
		    _sync = new OTS_ServerSynchronization(this);
		
		    Coordinator realCoord = null;

		    /*
		     * First register interposed-synchronization.
		     */
		
		    try
		    {
			OTS_ServerControl control = (OTS_ServerControl) super.controlHandle;

			if (controlHandle != null)
			{
			    realCoord = control.originalCoordinator();

			    if (realCoord != null)
			    {
				realCoord.register_synchronization(_sync.getSynchronization());
			    }
			    else
				throw new BAD_OPERATION(OTS_Exceptions.NO_TRANSACTION, CompletionStatus.COMPLETED_NO);
			}
			else
			    throw new BAD_OPERATION(OTS_Exceptions.NO_TRANSACTION, CompletionStatus.COMPLETED_NO);
		    }
		    catch (Inactive e1)
		    {
			realCoord = null;

			throw e1;
		    }
		    catch (SynchronizationUnavailable e2)
		    {
			realCoord = null;

			throw e2;
		    }
		    catch (SystemException e3)
		    {
			realCoord = null;

			throw e3;
		    }
		    
		    realCoord = null;
		}
	    }

	    /*
	     * Now register the synchronization locally.
	     */

	    super.register_synchronization(theSync);
	}
    }
    

    
    /*
     * These methods are here to make protected methods in OTS_Transaction
     * available to OTS_ServerSynchronization.
     */

public synchronized void doBeforeCompletion () throws SystemException
    {
	_beforeCompleted = true;

	super.beforeCompletion();
    }

public synchronized void doAfterCompletion (org.omg.CosTransactions.Status s) throws SystemException
    {
	super.afterCompletion(s);
    }

public static String typeName ()
    {
	return "/StateManager/BasicAction/OTS_Transaction/OTS_ServerTransaction";    
    }

public final synchronized void setRecoveryCoordinator (RecoveryCoordinator recCoord)
    {
	_recoveryCoordinator = recCoord;
    }

    /*
     * If this is a top-level transaction then we should have
     * a recovery coordinator reference, so save it away.
     */

public boolean save_state (OutputObjectState os, int ot)
    {
	try
	{
	    if (_recoveryCoordinator != null)
	    {
		os.packBoolean(true);
		os.packString(ORBInterface.orb().object_to_string(_recoveryCoordinator));
	    }
	    else
		os.packBoolean(false);

	    return super.save_state(os, ot);
	}
	catch (IOException e)
	{
	    ErrorStream.warning().println("OTS_ServerTransaction.save_state: "+e);
	}
	
	return false;
    }
    
public boolean restore_state (InputObjectState os, int ot)
    {
	try
	{
	    boolean haveRecCoord = os.unpackBoolean();
	    
	    if (haveRecCoord)
	    {
		try
		{
		    String ior = os.unpackString();
		    org.omg.CORBA.Object objRef = ORBInterface.orb().string_to_object(ior);
		    
		    _recoveryCoordinator = RecoveryCoordinatorHelper.narrow(objRef);
		}
		catch (Exception e)
		{
		    ErrorStream.warning().println("OTS_ServerTransaction.restoreState: "+e);
		    
		    return false;
		}
	    }
	    else
		_recoveryCoordinator = null;
	    
	    return super.restore_state(os, ot);
	}
	catch (IOException ex)
	{
	    ErrorStream.warning().println("OTS_ServerTransaction.restoreState: "+ex);
	}
	
	return false;
    }

public String toString ()
    {
	return "OTS_ServerTransaction < "+get_uid()+" >";
    }

    /**
     * Used during crash recovery. The Uid is the identity of the state
     * which this transaction's log is stored in. It is not the identity
     * of the transaction!
     */

protected OTS_ServerTransaction (Uid recoveringActUid)
    {
	super(recoveringActUid);
	
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PROTECTED,
					       FacilityCode.FAC_OTS, "OTS_ServerTransaction::OTS_ServerTransaction ( "+recoveringActUid+" )");
	}

	_savingUid = recoveringActUid;
	_sync = null;
	_beforeCompleted = false;
	_recoveryCoordinator = null;
	_prepState = ActionStatus.COMMITTING;
    }

protected final synchronized int preparedStatus ()
    {
	return _prepState;
    }
    
private final int returnStatus ()
    {
	int heuristic = super.getHeuristicDecision();
	
	switch (heuristic)
	{
	case PrepareOutcome.PREP_OK:
	case PrepareOutcome.FINISH_OK:
	    return super.status();
	case PrepareOutcome.HEURISTIC_ROLLBACK:
	    return ActionStatus.H_ROLLBACK;
	case PrepareOutcome.HEURISTIC_COMMIT:
	    return ActionStatus.H_COMMIT;
	case PrepareOutcome.HEURISTIC_MIXED:
	    return ActionStatus.H_MIXED;
	case PrepareOutcome.HEURISTIC_HAZARD:
	default:
	    return ActionStatus.H_HAZARD;
	}
    }

protected RecoveryCoordinator     _recoveryCoordinator;

private Uid                       _savingUid;
private OTS_ServerSynchronization _sync;
private boolean                   _beforeCompleted;
private int                       _prepState;
    
private static boolean _interposedSynch = false;

    static
    {
	String inter = PropertyManager.getProperty(OTS_Environment.OTS_SUPPORT_INTERPOSED_SYNCHRONIZATION);

	if (inter != null)
	{
	    if (inter.compareTo("YES") == 0)
		_interposedSynch = true;
	}	
    }
    
}
