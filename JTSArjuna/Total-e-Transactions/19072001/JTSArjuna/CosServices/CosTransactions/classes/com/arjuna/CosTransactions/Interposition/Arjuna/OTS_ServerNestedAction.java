/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Hewlett Packard Arjuna Labs,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_ServerNestedAction.javatmpl,v 1.4.6.1.2.2.8.2.6.1.2.1.2.1.24.1.8.1.4.1 2001/06/28 14:09:11 nmcl Exp $
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




package com.arjuna.CosTransactions.Interposition.Arjuna;

import com.arjuna.OrbCommon.*;
import com.arjuna.ArjunaCore.Thread.ThreadActionData;
import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.CosTransactions.*;
import com.arjuna.CosTransactions.Interposition.OTS_ServerControl;
import com.arjuna.CosTransactions.Interposition.OTS_ServerTransaction;
import org.omg.CosTransactions.*;
import org.omg.CORBA.CompletionStatus;

import org.omg.CosTransactions.HeuristicHazard;
import org.omg.CosTransactions.HeuristicMixed;
import org.omg.CORBA.SystemException;
import org.omg.CORBA.BAD_OPERATION;
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
 * An attempt at a portable way of including the OTS exceptions,
 * which are either mapped as system exceptions or user exceptions,
 * depending upon the Orb.
 */



import org.omg.CosTransactions.WrongTransaction;

import org.omg.CORBA.INVALID_TRANSACTION;
import org.omg.CORBA.TRANSACTION_REQUIRED;
import org.omg.CORBA.TRANSACTION_ROLLEDBACK;





/**
 * This looks like an atomic action, but is not actually derived from
 * BasicAction or OTS_Transaction. This is because of the way in which the
 * OTS creates and manipulates transactions.
 *
 * This is a nested action proxy.
 */

public class OTS_ServerNestedAction extends OTS_ServerResource implements  org.omg.CosTransactions .SubtransactionAwareResourceOperations 
{
    
    /*
     * Create local transactions with same ids as remote.
     */

public OTS_ServerNestedAction (OTS_ServerControl myControl)
    {
	super(myControl);

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "OTS_ServerNestedAction::OTS_ServerNestedAction ( "+_theUid+" )");
	}

	_theResource = null;
	_resourceRef = null;
	
	if (_theControl != null)
	{
	    _theResource = new  org.omg.CosTransactions. SubtransactionAwareResourcePOATie (this);
	    
	    OAInterface.objectIsReady( _theResource ) ;

	    _resourceRef =  org.omg.CosTransactions.SubtransactionAwareResourceHelper.narrow(OAInterface.corbaReference( _theResource )) ;
	    
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

	    if (!(_valid = registerSubTran(realCoordinator)))
	    {
		ErrorStream.stream(ErrorStream.WARNING).println("OTS_ServerNestedAction - could not register interposed hierarchy!");

		/*
		 * Failed to register. Valid is set, and the interposition controller
		 * will now deal with this.
		 */

		realCoordinator = null;
	    }
	}
    }

public void finalize ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.DESTRUCTORS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "OTS_ServerNestedAction.finalize ( "+_theUid+" )");
	}

	_theResource = null;
	_resourceRef = null;

	super.finalize();
    }
 
public void commit_subtransaction (Coordinator parent) throws SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "OTS_ServerNestedAction::commit_subtransaction : "+_theUid);
	}

	if (_theControl == null)
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("OTS_ServerNestedAction::commit_subtransaction - attempt to commit with null control!");


	    throw new INVALID_TRANSACTION (OTS_Exceptions.SERVERAA_NO_CONTROL, CompletionStatus.COMPLETED_NO);

	}

	if (_theControl.isWrapper())
	{
	    destroyResource();
	    return;
	}
		
	OTS_ServerTransaction theTransaction = (OTS_ServerTransaction) _theControl.getImplHandle();
	
	ThreadActionData.pushAction(theTransaction);
    
	/*
	 * Do nothing about propagation since we should already be
	 * registered with the parent, i.e., ignore the parent
	 * parameter.
	 */

	/*
	 * We should not get exceptions here.
	 */
	
	try
	{
	    theTransaction.commit(false);
	}
	catch (TRANSACTION_ROLLEDBACK  e1)
	{
	    if (DebugController.enabled())
		ErrorStream.stream(ErrorStream.WARNING).println("OTS_ServerNestedAction.commit_subtransaction: caught TransactionRolledBack");

	    ThreadActionData.popAction();
	    destroyResource();

	    throw e1;
	}
	catch (INVALID_TRANSACTION  e5)
	{
	    if (DebugController.enabled())
		ErrorStream.stream(ErrorStream.WARNING).println("OTS_ServerNestedAction.commit_subtransaction: caught InvalidTransaction");

	    ThreadActionData.popAction();
	    destroyResource();

	    throw e5;
	}
	catch (HeuristicMixed e2)
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("OTS_ServerNestedAction.commit_subtransaction: caught HeuristicMixed");

	    ThreadActionData.popAction();
	    destroyResource();
	    
	    /*
	     * Can't rethrow heuristic exceptions for subtransactions!
	     */
	    
	    throw new BAD_OPERATION(OTS_Exceptions.HEURISTIC_COMMIT, CompletionStatus.COMPLETED_MAYBE);
	}
	catch (HeuristicHazard e3)
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("OTS_ServerNestedAction.commit_subtransaction: caught HeuristicHazard");

	    ThreadActionData.popAction();
	    destroyResource();

	    throw new BAD_OPERATION(OTS_Exceptions.HEURISTIC_COMMIT, CompletionStatus.COMPLETED_MAYBE);
	}
	catch (SystemException e4)
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("OTS_ServerNestedAction.commit_subtransaction: caught unknown system exception! "+e4);

	    ThreadActionData.popAction();
	    destroyResource();

	    throw e4;
	}
	catch (Exception e5)
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("OTS_ServerNestedAction.commit_subtransaction: caught unknown exception! "+e5);

	    ThreadActionData.popAction();
	    destroyResource();

	    throw new UNKNOWN(e5.toString());
	}

	ThreadActionData.popAction();
	destroyResource();
    }

public void rollback_subtransaction () throws SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "OTS_ServerNestedAction::rollback_subtransaction : "+_theUid);
	}

	if (_theControl == null)
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("OTS_ServerNestedAction::rollback_subtransaction - attempt to commit with null control!");


	    throw new INVALID_TRANSACTION (OTS_Exceptions.SERVERAA_NO_CONTROL, CompletionStatus.COMPLETED_NO);

	}

	if (_theControl.isWrapper())
	{
	    destroyResource();
	    return;
	}

	OTS_ServerTransaction theTransaction = (OTS_ServerTransaction) _theControl.getImplHandle();
	
	ThreadActionData.pushAction(theTransaction);

	try
	{
	    if (!valid())
		theTransaction.doPhase2Abort();
	    else
		theTransaction.rollback();
	}
	catch (SystemException e)
	{
	    ThreadActionData.popAction();
	    destroyResource();

	    throw e;
	}
	catch (Exception ex)
	{
	    ThreadActionData.popAction();
	    destroyResource();

	    throw new UNKNOWN(ex.toString());
	}

	ThreadActionData.popAction();
	destroyResource();
    }

    /*
     * These methods should never be called.
     */


public org.omg.CosTransactions.Vote prepare () throws SystemException, HeuristicMixed, HeuristicHazard

    {
	throw new BAD_OPERATION(OTS_Exceptions.SERVERAA_PREPARE, CompletionStatus.COMPLETED_NO);
    }

public void rollback () throws SystemException, HeuristicCommit, HeuristicMixed, HeuristicHazard
    {
    }
    
public void commit () throws SystemException, NotPrepared, HeuristicRollback, HeuristicMixed, HeuristicHazard
    {
    }

public void forget () throws SystemException
    {
    }



public void commit_one_phase () throws HeuristicHazard, SystemException


    {
    }

public SubtransactionAwareResource theResource ()
    {
	return _resourceRef;
    }
 
protected OTS_ServerNestedAction ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PROTECTED,
					       FacilityCode.FAC_OTS, "OTS_ServerNestedAction::OTS_ServerNestedAction ()");
	}

	_theResource = null;
	_resourceRef = null;
    }

protected final synchronized void destroyResource ()
    {
	if (!_destroyed)
	{
	    _destroyed = true;
	    
	    if (_parent != null)
	    {
		/*
		 * Now try to garbage collect this resource. Since it was
		 * registered as a subtranaware resource it won't get called
		 * again.
		 */
	    
		if (!_parent.removeChild(this))
		    ErrorStream.stream(ErrorStream.WARNING).println("Could not remove child "+get_uid()+" from "+_parent.get_uid());
	    }

	    if (_theResource != null)
	    {
		OAInterface.shutdownObject( _theResource ) ;
		_theResource = null;
	    }
	}
    }

protected boolean registerSubTran (Coordinator theCoordinator)
    {
	boolean result = false;

	if (theCoordinator != null)
	{
	    try
	    {
		theCoordinator.register_subtran_aware(_resourceRef);
		result = true;
	    }
	    catch (Inactive e)
	    {
		ErrorStream.stream(ErrorStream.WARNING).println("OTS_ServerNestedAction::registerSubTran - Inactive exception");
	    }
	    catch (NotSubtransaction e)
	    {
		ErrorStream.stream(ErrorStream.WARNING).println("OTS_ServerNestedAction::registerSubTran - NotSubtransaction exception");
	    }
	    catch (SystemException e)
	    {
		ErrorStream.stream(ErrorStream.WARNING).println("OTS_ServerNestedAction::registerSubTran - caught unexpected exception: "+e);
	    }
	}
	else
	    ErrorStream.stream(ErrorStream.WARNING).println("OTS_ServerTopLevelAction.registerSubTran - no coordinator to use!");

	return result;
    }

protected  org.omg.CosTransactions. SubtransactionAwareResourcePOATie  _theResource;
protected SubtransactionAwareResource _resourceRef;
 
};
