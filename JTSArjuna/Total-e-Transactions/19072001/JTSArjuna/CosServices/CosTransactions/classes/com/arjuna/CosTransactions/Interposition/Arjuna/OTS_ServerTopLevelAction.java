/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Hewlett Packard Arjuna Labs,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_ServerTopLevelAction.javatmpl,v 1.6.2.1.4.1.2.2.2.2.6.13.6.1.2.1.2.2.24.2.8.1.4.1 2001/06/28 14:09:11 nmcl Exp $
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
































































package com.arjuna.CosTransactions.Interposition.Arjuna;

import com.arjuna.OrbCommon.*;
import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaCore.Atomic.PrepareOutcome;
import com.arjuna.ArjunaCore.Atomic.ActionStatus;
import com.arjuna.ArjunaCore.Thread.ThreadActionData;
import com.arjuna.ArjunaCore.Interface.ObjectStore;
import com.arjuna.CosTransactions.*;
import com.arjuna.CosTransactions.Interposition.OTS_ServerControl;
import com.arjuna.CosTransactions.Interposition.OTS_ServerTransaction;
import org.omg.CosTransactions.*;
import org.omg.CORBA.CompletionStatus;

import com.arjuna.ArjunaCore.Common.ObjectStoreException;
import org.omg.CORBA.OBJECT_NOT_EXIST;
import org.omg.CORBA.BAD_PARAM;
import org.omg.CORBA.BAD_OPERATION;
import org.omg.CORBA.SystemException;
import org.omg.CORBA.UNKNOWN;


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





/*
 * This looks like an atomic action, but is not actually derived from
 * BasicAction or OTS_Transaction. This is because of the way in which the
 * OTS creates and manipulates transactions.
 *
 * As with OTS_Transaction, we only create actions here, and do not associated
 * these contexts with any thread. We do the association later.
 *
 * If we were to do the creation via a suitably modified current interface
 * then the thread association would be done for us automatically, and we
 * would not have to call resume at all.
 *
 * This is a top-level action proxy.
 */

public class OTS_ServerTopLevelAction extends OTS_ServerResource implements  org.omg.CosTransactions .ResourceOperations 
{

public OTS_ServerTopLevelAction (OTS_ServerControl control)
    {
	super(control);

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "OTS_ServerTopLevelAction::OTS_ServerTopLevelAction ( "+_theUid+" )");
	}

	_theResource = null;
	_resourceRef = getReference();

	/*
	 * Now attempt to do interposition registration.
	 */

	if (_resourceRef != null)
	{
	    /*
	     * Would like to be able to attach a thread filter
	     * to this object if process-filters aren't supported.
	     * However, currently this won't work as we can't have
	     * two different filter types working at the same
	     * time.
	     *
	     *		ATTACH_THREAD_FILTER_(_theResource);
	     */
	    
	    Coordinator realCoordinator = _theControl.originalCoordinator();

	    if (!(_valid = registerResource(realCoordinator)))
	    {
		ErrorStream.stream(ErrorStream.WARNING).println("OTS_ServerTopLevelAction - could not register interposed hierarchy!");
	    
		/*
		 * Failed to register. Valid is set, and the interposition
		 * controller will now deal with this.
		 */
	    }

	    realCoordinator = null;
	}
	else
	    _valid = false;
    }
 
/*
 * Assume only one thread can delete an object!
 */
 
public void finalize ()
    {
	_theResource = null;
	_resourceRef = null;

	super.tidyup();

	super.finalize();
    }

public Resource getReference ()
    {
	if ((_resourceRef == null) && _valid)
	{
	    if (_theControl != null)
	    {
		_theResource = new  org.omg.CosTransactions. ResourcePOATie (this);
	    
		OAInterface.objectIsReady( _theResource ) ;

		_resourceRef =  org.omg.CosTransactions.ResourceHelper.narrow(OAInterface.corbaReference( _theResource )) ;
	    }
	    else
		_valid = false;
	}
	
	return _resourceRef;
    }
 
/*
 * Will only be called by the remote top-level transaction.
 */


public org.omg.CosTransactions.Vote prepare () throws HeuristicMixed, HeuristicHazard, SystemException

    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "OTS_ServerTopLevelAction::prepare for "+_theUid);
	}

	if (_theControl == null)
	{

	    throw new INVALID_TRANSACTION (OTS_Exceptions.SERVERAA_NO_CONTROL, CompletionStatus.COMPLETED_NO);

	}

	if (_theControl.isWrapper())
	{
	    destroyResource();  // won't necessarily get another invocation!
	    return Vote.VoteReadOnly;
	}

	OTS_ServerTransaction theTransaction = (OTS_ServerTransaction) _theControl.getImplHandle();
	
	ThreadActionData.pushAction(theTransaction); // unnecessary if context goes with all calls.

	int result = PrepareOutcome.PREP_NOTOK;

	/*
	 * Transaction may have locally timed out and been rolled back.
	 */

	int s = theTransaction.status();

	if ((s == ActionStatus.RUNNING) || (s == ActionStatus.ABORT_ONLY))
	    result = theTransaction.doPrepare();
	else
	{
	    switch (s)
	    {
	    case ActionStatus.COMMITTING:
	    case ActionStatus.COMMITTED:
	    case ActionStatus.H_COMMIT:
		result = PrepareOutcome.PREP_OK;
		break;
	    case ActionStatus.H_MIXED:
		result = PrepareOutcome.HEURISTIC_MIXED;
		break;
	    case ActionStatus.H_HAZARD:
		result = PrepareOutcome.HEURISTIC_HAZARD;
		break;
	    }
	}
    
	ThreadActionData.popAction();

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "OTS_ServerTopLevelAction::prepare for "+_theUid+" : "+PrepareOutcome.printString(result));
	}
	
	switch (result)
	{
	case PrepareOutcome.INVALID_TRANSACTION:

	    throw new INVALID_TRANSACTION (OTS_Exceptions.INVALID_ACTION, CompletionStatus.COMPLETED_NO);

	case PrepareOutcome.PREP_OK:
	    return Vote.VoteCommit;
	case PrepareOutcome.PREP_NOTOK:
	    return Vote.VoteRollback;
	case PrepareOutcome.PREP_READONLY:
	    destroyResource();  // won't necessarily get another invocation!
	    
	    // what is we subsequently rollback?
	    
	    return Vote.VoteReadOnly;
	case PrepareOutcome.HEURISTIC_MIXED:
	    throw new HeuristicMixed();  // will eventually get forget
	case PrepareOutcome.HEURISTIC_HAZARD:
	default:
	    throw new HeuristicHazard();
	}
    }

public void rollback () throws HeuristicCommit, HeuristicMixed, HeuristicHazard, SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "OTS_ServerTopLevelAction::rollback for "+_theUid);
	}

	if (_theControl == null)
	{

	    throw new INVALID_TRANSACTION (OTS_Exceptions.SERVERAA_NO_CONTROL, CompletionStatus.COMPLETED_NO);

	}

	if (_theControl.isWrapper())
	{
	    destroyResource();
	    return;
	}
	
	OTS_ServerTransaction theTransaction = (OTS_ServerTransaction) _theControl.getImplHandle();
	
	ThreadActionData.pushAction(theTransaction);

	int actionStatus = theTransaction.status();
    
	if (actionStatus == ActionStatus.PREPARED)
	{
	    /*
	     * This will also call any after_completions on
	     * registered synchronizations.
	     */

	    actionStatus = theTransaction.doPhase2Abort();
	}
	else
	{
	    if (actionStatus == ActionStatus.RUNNING)
	    {
		try
		{
		    /*
		     * Have to do this because of the way PI works
		     * with thread-context association.
		     */

		    if (!valid())
			theTransaction.doPhase2Abort();  // must rollback
		    else
			theTransaction.rollback();

		    actionStatus = ActionStatus.ABORTED;
		}
		catch (SystemException ex)
		{
		    actionStatus = ActionStatus.ABORTED;

		    throw ex;
		}
	    }
	}

	ThreadActionData.popAction();

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "OTS_ServerTopLevelAction::rollback for "+_theUid+" : "+ActionStatus.printString(actionStatus));
	}

	switch (actionStatus)
	{
	case ActionStatus.PREPARED:

	    throw new INVALID_TRANSACTION (OTS_Exceptions.INVALID_ACTION, CompletionStatus.COMPLETED_NO);

	case ActionStatus.ABORTED:
	case ActionStatus.H_ROLLBACK:
	    destroyResource();
	    break;
	case ActionStatus.COMMITTED:
	case ActionStatus.H_COMMIT:
	    throw new HeuristicCommit();
	case ActionStatus.H_MIXED:
	    throw new HeuristicMixed();
	case ActionStatus.H_HAZARD:
	    throw new HeuristicHazard();
	default:
	    destroyResource();
	    break;
	}
    }

public void commit () throws NotPrepared, HeuristicRollback, HeuristicMixed, HeuristicHazard, SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "OTS_ServerTopLevelAction::commit for "+_theUid);
	}

	if (_theControl == null)
	{

	    throw new INVALID_TRANSACTION (OTS_Exceptions.SERVERAA_NO_CONTROL, CompletionStatus.COMPLETED_NO);

	}
	
	if (_theControl.isWrapper())
	{
	    destroyResource();
	    return;
	}

	OTS_ServerTransaction theTransaction = (OTS_ServerTransaction) _theControl.getImplHandle();
	
	ThreadActionData.pushAction(theTransaction);
	
	int actionStatus = theTransaction.status();
	boolean notPrepared = false;

	if (actionStatus == ActionStatus.PREPARED)
	{
	    /*
	     * This will also call any after_completions on
	     * registered synchronizations.
	     */
	    
	    actionStatus = theTransaction.doPhase2Commit(false);
	}
	else
	{
	    if (actionStatus == ActionStatus.RUNNING)
	    {
		if (DebugController.enabled())
		{
		    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						       FacilityCode.FAC_OTS, "OTS_ServerTopLevelAction::commit for "+_theUid+" : NotPrepared");
		}

		notPrepared = true;
	    }
	}

	ThreadActionData.popAction();

	if (notPrepared)
	    throw new NotPrepared();

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "OTS_ServerTopLevelAction::commit for "+_theUid+" : "+ActionStatus.printString(actionStatus));
	}
	
	switch (actionStatus)
	{
	case ActionStatus.PREPARED:

	    throw new INVALID_TRANSACTION (OTS_Exceptions.SERVERAA_NO_CONTROL, CompletionStatus.COMPLETED_NO);

	case ActionStatus.COMMITTED:
	case ActionStatus.H_COMMIT:
	    destroyResource();
	    break;
	case ActionStatus.ABORTED:
	case ActionStatus.H_ROLLBACK:
	    throw new HeuristicRollback();
	case ActionStatus.H_MIXED:
	    throw new HeuristicMixed();
	case ActionStatus.H_HAZARD:
	    throw new HeuristicHazard();
	default:
	    destroyResource();
	    break;
	}
    }

public void forget () throws SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "OTS_ServerTopLevelAction::forget for "+_theUid);
	}

	boolean forgot = true;
	
	if (_theControl != null)
	    forgot = _theControl.forgetHeuristics();

	destroyResource();  // causes the removal of the transaction state.

	if (!forgot)
	    throw new BAD_OPERATION();
    }



public void commit_one_phase () throws HeuristicHazard, SystemException


    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "OTS_ServerTopLevelAction::commit_one_phase for "+_theUid);
	}

	if (_theControl == null)
	{

	    throw new INVALID_TRANSACTION (OTS_Exceptions.SERVERAA_NO_CONTROL, CompletionStatus.COMPLETED_NO);

	}

	if (_theControl.isWrapper())
	{
	    destroyResource();
	    return;
	}

	OTS_ServerTransaction theTransaction = (OTS_ServerTransaction) _theControl.getImplHandle();

	if (theTransaction == null)
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("OTS_ServerTopLevelAction.commit_one_phase - no transaction!");


	    throw new INVALID_TRANSACTION (OTS_Exceptions.NO_TRANSACTION, CompletionStatus.COMPLETED_NO);

	}
	
	ThreadActionData.pushAction(theTransaction);

	try
	{
	    /*
	     * This will commit and do any before/after_completion calls
	     * on registered synchronizations.
	     */

	    theTransaction.doCommit(true);
	}
	catch (HeuristicHazard e1)
	{
	    /*
	     * Is a heuristic, then don't remove the
	     * transaction information.
	     */

	    ThreadActionData.popAction();

	    throw e1;
	}

	catch (TRANSACTION_ROLLEDBACK  e4)
	{
	    ThreadActionData.popAction();

	    destroyResource();

	    throw e4;
	}
	catch (INVALID_TRANSACTION  e5)
	{
	    ThreadActionData.popAction();

	    destroyResource();

	    throw e5;
	}
	catch (SystemException e6)
	{
	    ThreadActionData.popAction();

	    destroyResource();

	    throw e6;
	}
	catch (Exception e7)
	{
	    ThreadActionData.popAction();

	    destroyResource();

	    throw new UNKNOWN(e7.toString());
	}

	ThreadActionData.popAction();

	destroyResource();
    }

public String type ()
    {
	return "/Resources/Arjuna/OTS_ServerTopLevelAction";
    }

protected OTS_ServerTopLevelAction ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PROTECTED,
					       FacilityCode.FAC_OTS, "OTS_ServerTopLevelAction::OTS_ServerTopLevelAction ()");
	}

	_theResource = null;
	_resourceRef = null;
    }

protected synchronized void destroyResource ()
    {
	if (!_destroyed)
	{
	    try
	    {
		if (OTS_Interposition.destroy(get_uid()))
		    _destroyed = true;
	    }
	    catch (Exception e)
	    {
		ErrorStream.warning().println("OTS_ServerTopLevelAction.destroyResource: "+e);
	    }
	    
	    try
	    {
		if (_theResource != null)
		{
		    OAInterface.shutdownObject( _theResource ) ;
		    _theResource = null;
		}
	    }
	    catch (Exception e)
	    {
		ErrorStream.warning().println("OTS_ServerTopLevelAction.destroyResource: "+e);
	    }
	}
    }
 
protected boolean registerResource (Coordinator theCoordinator)
    {
	boolean result = false;

	if (theCoordinator != null)
	{
	    try
	    {
		/*
		 * Register resource and pass RecoveryCoordinator reference
		 * to the interposed transaction to save and restore.
		 */

		RecoveryCoordinator recoveryCoord = theCoordinator.register_resource(_resourceRef);

		if (!_theControl.isWrapper())
		{
		    OTS_ServerTransaction tx = (OTS_ServerTransaction) _theControl.getImplHandle();
		
		    if (tx != null)
		    {
			tx.setRecoveryCoordinator(recoveryCoord);

			result = true;
		    }
		    else
			result = false;
		}
		else
		    result = true;
	    }
	    catch (ClassCastException classCastException)
	    {
		ErrorStream.stream(ErrorStream.WARNING).println("OTS_ServerTopLevelAction.registerResource - ClassCastException of " + _theControl.getImplHandle().getClass());
	    }
	    catch (Inactive ine)
	    {
		ErrorStream.stream(ErrorStream.WARNING).println("OTS_ServerTopLevelAction.registerResource - Inactive exception");
	    }
	    catch (TRANSACTION_ROLLEDBACK  ex1)
	    {
		ErrorStream.stream(ErrorStream.WARNING).println("OTS_ServerTopLevelAction.registerResource - caught TransactionRolledBack");
	    }
	    catch (INVALID_TRANSACTION  ex2)
	    {
		ErrorStream.stream(ErrorStream.WARNING).println("OTS_ServerTopLevelAction.registerResource - caught InvalidTransaction");
	    }
	    catch (Exception e)
	    {
		ErrorStream.stream(ErrorStream.WARNING).println("OTS_ServerTopLevelAction.registerResource - caught unexpected exception: "+e);
	    }
	}
	else
	    ErrorStream.stream(ErrorStream.WARNING).println("OTS_ServerTopLevelAction.registerResource - no coordinator to use!");
    
	return result;
    }

protected  org.omg.CosTransactions. ResourcePOATie  _theResource;
protected Resource                                               _resourceRef;

};
