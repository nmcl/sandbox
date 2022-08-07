/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: AtomicTransaction.javatmpl,v 1.7.4.1.6.2.2.1.6.1.10.1.4.2.2.1.14.1.2.1 2001/04/23 11:51:24 nmcl Exp $
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




package com.arjuna.CosTransactions.ArjunaOTS;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.CosTransactions.OTS;
import com.arjuna.CosTransactions.OTS_Current;
import com.arjuna.CosTransactions.OTS_Exceptions;
import com.arjuna.CosTransactions.OTS_Utility;
import org.omg.CosTransactions.*;
import org.omg.CORBA.CompletionStatus;

import org.omg.CosTransactions.NoTransaction;
import org.omg.CosTransactions.Unavailable;
import org.omg.CosTransactions.InvalidControl;
import org.omg.CosTransactions.HeuristicMixed;
import org.omg.CosTransactions.HeuristicHazard;

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





import org.omg.CORBA.SystemException;
import org.omg.CORBA.UNKNOWN;
import org.omg.CORBA.BAD_OPERATION;

/**
 * Similar to CosTransactions::Current. However, this class does transaction
 * scoping, so if an instance is garbage collected and the transaction is still
 * running, it will be rolled back automatically. It also adds some
 * convenience routines which Current does not do.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: AtomicTransaction.javatmpl,v 1.7.4.1.6.2.2.1.6.1.10.1.4.2.2.1.14.1.2.1 2001/04/23 11:51:24 nmcl Exp $
 * @since JTS 1.0.
 */

public class AtomicTransaction
{

    /**
     * The types of transactions which can be created.
     */

public static final int TOP_LEVEL = 0;
public static final int NESTED = 1;

    /**
     * Create a new transaction. It is not running at this stage.
     */

public AtomicTransaction ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "AtomicTransaction::AtomicTransaction ()");
	}

	_theAction = null;
	_theStatus = Status.StatusNoTransaction;
	_timeout = get_timeout();
    }

public void finalize ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.DESTRUCTORS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "AtomicTransaction.finalize ()");
	}

	if (_theAction != null)
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("AtomicTransaction - running atomic action going out of scope. Will abort.");
	    
	    String name = null;

	    try
	    {
		name = get_transaction_name();
	    }
	    catch (SystemException ex)
	    {
		ErrorStream.stream(ErrorStream.WARNING).println("Cannot determine transaction name!");
	    }

	    try
	    {
		rollback();  // tidies up for us.
	    }
	    catch (NoTransaction e)
	    {
		ErrorStream.stream(ErrorStream.WARNING).println("Could not abort transaction "+name);
	    }
	    catch (Exception e)
	    {
		ErrorStream.stream(ErrorStream.WARNING).println("Could not abort transaction "+name);
	    }
	}
    }

    /**
     * @return the transaction name. Should only be used for debugging
     * purposes.
     */

public synchronized String get_transaction_name () throws SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "AtomicTransaction::get_transaction_name ()");
	}

	String name = null;

	if (_theAction != null)
	{
	    try
	    {
		Coordinator c = _theAction.get_coordinator();

		if (c != null)
		    name = c.get_transaction_name();

		c = null;
	    }
	    catch (Unavailable e)
	    {
		ErrorStream.stream(ErrorStream.WARNING).println("AtomicTransaction::get_transaction_name - transaction Unavailable.");
		
		throw new UNKNOWN(OTS_Exceptions.UNKNOWN_EXCEPTION, CompletionStatus.COMPLETED_NO);
	    }
	}

	return name;
    }

    /**
     * Start the transaction.
     *
     * @exception org.omg.CosTransactions.SubtransactionsUnavailable if
     * subtransactions have been disabled, and the invoking thread already
     * has a transaction associated with it.
     *
     * @exception org.omg.CORBA.INVALID_TRANSACTION if the transaction has
     * already begun or has completed.
     */
    

public synchronized void begin () throws SubtransactionsUnavailable, INVALID_TRANSACTION , SystemException

    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "AtomicTransaction::begin ()");
	}

	// already begun?

	if (_theAction != null)
	{

	    throw new INVALID_TRANSACTION (OTS_Exceptions.ALREADY_BEGUN, CompletionStatus.COMPLETED_NO);

	}

	OTS_Current current = OTS.current();

	current.begin();

	_theAction = current.get_control();
	_theStatus = current.get_status();

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "AtomicTransaction::begin create "+_theAction);
	}
    }
    
    /*
     * Commit the transaction. If the current transaction associated
     * with the thread is not this transaction, then this transaction
     * is rolled back and leave current alone.
     *
     * @param report_heuristics indicates whether heuristic reporting
     * is desired.
     *
     * @exception org.omg.CosTransactions.NoTransaction if the transaction
     * has already been terminated.
     *
     * @exception org.omg.CORBA.TRANSACTION_ROLLEDBACK if the transaction
     * rolls back.
     *
     * @exception org.omg.CosTransactions.HeuristicMixed if some of the
     * transaction participants committed, while some rolled back.
     *
     * @exception org.omg.CosTransactions.HeuristicHazard if some of the
     * transaction participants committed, some rolled back, and the
     * outcome of others is indeterminate.
     *
     * @exception org.omg.CORBA.WRONG_TRANSACTION if the current transaction
     * is not this transaction.
     */
    


public synchronized void commit (boolean report_heuristics) throws NoTransaction, HeuristicMixed, HeuristicHazard, WrongTransaction, SystemException


    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "AtomicTransaction::commit ( "+report_heuristics+" ) for "+_theAction);
	}

	if (_theAction == null)
	{
	    throw new NoTransaction();
	}

	if (!validTransaction())
	{

	    throw new BAD_OPERATION(OTS_Exceptions.NOT_CURRENT_TRANSACTION, CompletionStatus.COMPLETED_NO);

	}
	
	/*
	 * OK to use current since we have just guaranteed that the
	 * transaction is the same as current. Use current rather than
	 * saved control since it will do thread tracking for us.
	 */

	OTS_Current current = OTS.current();
	
	/*
	 * Release our handle first, since current is about
	 * to destroy the action control.
	 */
	
	_theAction = null;

	try
	{
	    current.commit(report_heuristics);

	    _theStatus = Status.StatusCommitted;
	}
	catch (NoTransaction e)
	{
	    _theStatus = Status.StatusNoTransaction;

	    throw e;
	}
	catch (HeuristicMixed e)
	{
	    _theStatus = getStatus();
	    
	    throw e;
	}
	catch (HeuristicHazard e)
	{
	    _theStatus = getStatus();
	    
	    throw e;
	}
	catch (TRANSACTION_ROLLEDBACK  e)
	{
	    _theStatus = Status.StatusRolledBack;
	    
	    throw e;
	}
	catch (SystemException e)
	{
	    _theStatus = getStatus();
	    
	    throw e;
	}
    }

    /*
     * Rollback the transaction. If the current transaction associated
     * with the thread is not this transaction, then this transaction
     * is rolled back and leave current alone.
     *
     * @exception org.omg.CosTransactions.NoTransaction if the transaction
     * has already been terminated.
     *
     * @exception org.omg.CORBA.WRONG_TRANSACTION if the current transaction
     * is not this transaction.
     */


public synchronized void rollback () throws NoTransaction, WrongTransaction, SystemException

    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "AtomicTransaction::rollback for "+_theAction);
	}

	if (_theAction == null)
	{
	    throw new NoTransaction();
	}

	if (!validTransaction())
	{

	    throw new BAD_OPERATION(OTS_Exceptions.NOT_CURRENT_TRANSACTION, CompletionStatus.COMPLETED_NO);

	}
	
	/*
	 * OK to use current since we have just guaranteed that the
	 * transaction is the same as current. Use current rather than
	 * saved control since it will do thread tracking for us.
	 */

	OTS_Current current = OTS.current();

	_theAction = null;

	try
	{
	    current.rollback();
	}
	catch (NoTransaction e)
	{
	    _theStatus = Status.StatusNoTransaction;

	    throw e;
	}
	catch (TRANSACTION_ROLLEDBACK  e)
	{
	    _theStatus = Status.StatusRolledBack;
	    
	    throw e;
	}
	catch (SystemException e)
	{
	    _theStatus = getStatus();
	    
	    throw e;
	}
    }

    /**
     * Set the transaction timeout. This is the same as calling
     * org.omg.CosTransactions.Current.set_timeout().
     */

public void set_timeout (int seconds) throws SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "AtomicTransaction::set_timeout ( "+seconds+" )");
	}

	OTS_Current current = OTS.current();

	if (current != null)
	    current.set_timeout(seconds);
	else
	    throw new UNKNOWN();
    }

    /**
     * @return the timeout associated with transactions created by the
     * current thread.
     */

public int get_timeout () throws SystemException
    {
	OTS_Current current = OTS.current();

	if (current != null)
	{
	    int val = current.get_timeout();

	    if (DebugController.enabled())
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						   FacilityCode.FAC_OTS, "AtomicTransaction::get_timeout returning "+val);
	    }

	    return val;
	}
	else
	    throw new UNKNOWN();
    }

    /**
     * @return the timeout associated with this transaction.
     */

public int getTimeout () throws SystemException
    {
	return _timeout;
    }

    /**
     * @return the propagation context for this transaction.
     *
     * @since JTS 2.1.
     *
     * @exception org.omg.CosTransactions.Inactive if the current transaction
     * is no longer in the active phase.
     */

public PropagationContext get_txcontext () throws Inactive, SystemException
    {
	if (_theAction == null)
	{
	    throw new Inactive();
	}
	else
	{
	    try
	    {
		return _theAction.get_coordinator().get_txcontext();
	    }
	    catch (NullPointerException ex)
	    {
		throw new Inactive();
	    }
	    catch (Exception e)
	    {
		throw new BAD_OPERATION(e.toString());
	    }
	}
    }
    
    /**
     * Register the specified resource with this transaction.
     *
     * @return the org.omg.CosTransactions.RecoveryCoordinator reference
     * that can be used to later query the outcome of the transaction.
     *
     * @exception org.omg.CosTransactions.Inactive if the current transaction
     * is no longer in the active phase.
     */

public synchronized RecoveryCoordinator registerResource (Resource r) throws Inactive, SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "AtomicTransaction::registerResource ( "+r+" )");
	}

	RecoveryCoordinator rc = null;

	if (_theAction == null)
	{
	    throw new Inactive();
	}
	else
	{
	    Coordinator coord = null;

	    try
	    {
		coord = _theAction.get_coordinator();
	    }
	    catch (Unavailable e)
	    {
		coord = null;
	    }

	    if (coord != null)
	    {
		rc = coord.register_resource(r);

		coord = null;
	    }
	    else
		throw new UNKNOWN(OTS_Exceptions.INACTIVE_TRANSACTION, CompletionStatus.COMPLETED_NO);  // what else to raise?
	}

	return rc;
    }

    /**
     * Register the specified subtransaction aware resource with this
     * transaction. This transaction must be a subtransaction.
     *
     * @exception org.omg.CosTransactions.Inactive if this transaction is
     * no longer in the active phase.
     *
     * @exception org.omg.CosTransactions.NotSubtransaction if this transaction
     * is not a subtransaction.
     */

public synchronized void registerSubtranAware (SubtransactionAwareResource r) throws Inactive, NotSubtransaction, SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "AtomicTransaction::registerSubtranAware ( "+r+" )");
	}

	if (_theAction == null)
	{
	    throw new Inactive();
	}
	else
	{
	    Coordinator coord = null;

	    try
	    {
		coord = _theAction.get_coordinator();
	    }
	    catch (Unavailable e)
	    {
		coord = null;
	    }

	    if (coord != null)
	    {
		if (!coord.is_top_level_transaction())
		{
		    coord.register_subtran_aware(r);
		    coord = null;
		}
		else
		{
		    coord = null;

		    throw new NotSubtransaction();
		}
	    }
	    else
		throw new UNKNOWN(OTS_Exceptions.INACTIVE_TRANSACTION, CompletionStatus.COMPLETED_NO); // what else to raise?
	}
    }

    /**
     * Register the specified synchronization with this transaction. This
     * transaction must be a top-level transaction.
     *
     * @exception org.omg.CosTransactions.Inactive if this transaction is
     * no longer in the active phase.
     *
     * @exception org.omg.CosTransactions.SynchronizationUnavailable if this
     * transaction it not a top-level transaction.
     */

public synchronized void registerSynchronization (Synchronization sync) throws Inactive, SynchronizationUnavailable, SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "AtomicTransaction::registerSynchronization ( "+sync+" )");
	}


	if (_theAction == null)
	{
	    throw new Inactive();
	}
	else
	{
	    Coordinator coord = null;

	    try
	    {
		coord = _theAction.get_coordinator();
	    }
	    catch (Exception e)
	    {
		coord = null;
	    }

	    if (coord != null)
	    {
		coord.register_synchronization(sync);

		coord = null;
	    }
	    else
		throw new UNKNOWN(OTS_Exceptions.INACTIVE_TRANSACTION, CompletionStatus.COMPLETED_NO);  // what else to raise?
	}

    }
    
    /*
     * Should probably remove ability to get control as this allows
     * a user to commit/abort outside the control of the AtomicTransaction.
     */

    /**
     * @return the org.omg.CosTransactions.Control reference to this
     * transaction.
     */

public synchronized Control control () throws NoTransaction, SystemException
    {
	if (_theAction == null)
	{
	    throw new NoTransaction();
	}
	else
	    return _theAction;
    }

    /**
     * Suspend this transaction from the current thread. This transaction
     * must be active on the calling thread for this method to succeed.
     *
     * @exception org.omg.CosTransactions.NoTransaction if there is no
     * current transaction.
     *
     * @exception org.omg.CORBA.WRONG_TRANSACTION if the transaction associated
     * with the current thread is different from this thread.
     */


public synchronized void suspend () throws NoTransaction, WrongTransaction, SystemException

    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "AtomicTransaction::suspend called for "+_theAction);
	}

	if (_theAction != null)
	    throw new NoTransaction();

	if (!validTransaction())
	{

	    throw new BAD_OPERATION(OTS_Exceptions.NOT_CURRENT_TRANSACTION, CompletionStatus.COMPLETED_NO);

	}

	_theAction = null;

	_theAction = OTS.current().suspend();
    }

    /**
     * Resume this transaction.
     *
     * @exception org.omg.CosTransactions.InvalidControl if this transaction
     * is invalid.
     */
public synchronized void resume () throws InvalidControl, SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "AtomicTransaction::resume called for "+_theAction);
	}

	if (_theAction == null)
	    throw new InvalidControl();
	
	OTS.current().resume(_theAction);
    }

    /**
     * @return the status of this transaction.
     */


public synchronized org.omg.CosTransactions.Status get_status () throws SystemException

    {
	_theStatus = getStatus();
	
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "AtomicTransaction::get_status called for "+_theAction+" returning "+OTS_Utility.printStatus(_theStatus));
	}

	return _theStatus;
    }

    /**
     * Allow action commit to be supressed.
     */
    
public synchronized void rollbackOnly () throws SystemException, NoTransaction
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "AtomicTransaction::rollbackOnly called for "+_theAction);
	}

	if (_theAction != null)
	{
	    Coordinator c = null;

	    try
	    {
		c = _theAction.get_coordinator();
		
		c.rollback_only();
		
		c = null;
	    }
	    catch (Inactive e)
	    {
		c = null;
		    
		throw new NoTransaction();
	    }
	    catch (Unavailable e)
	    {
		throw new NoTransaction();
	    }
	}
	else
	    throw new NoTransaction();
    }
    
    /**
     * If this transaction current?
     * Assume we have checked that we are actually
     * a transaction!
     *
     * If not valid then abort this transaction here. Leave
     * current alone.
     */

protected final boolean validTransaction ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PROTECTED,
					       FacilityCode.FAC_OTS, "AtomicTransaction::validTransaction called for "+_theAction);
	}

	Coordinator coord = null;
	Control currControl = null;
	Coordinator currCoord = null;
	boolean valid = false;
	OTS_Current current = OTS.current();
    
	try
	{
	    if (_theAction != null)
		coord = _theAction.get_coordinator();
	    else
	    {
		ErrorStream.warning().println("AtomicTransaction.validTransaction - no transaction!");
		
		return false;
	    }
	    
	    currControl = current.get_control();

	    if (currControl != null)
	    {
		currCoord = currControl.get_coordinator();
		valid = coord.is_same_transaction(currCoord);
	    }

	    if ((currControl == null) || ((currCoord == null)))
	    {
		ErrorStream.stream(ErrorStream.WARNING).println("AtomicTransaction - current transaction is null!");
		valid = false;
	    }
	
	    if (!valid)
	    {
		String transactionName = get_transaction_name();
		String currentTransactionName = ((currCoord != null) ? currCoord.get_transaction_name() : "Unknown");
		Terminator term = _theAction.get_terminator();

		ErrorStream.stream(ErrorStream.WARNING).print("AtomicTransaction - terminated out of sequence "+transactionName+".");
		ErrorStream.stream().println(" Will abort. Current transaction is "+currentTransactionName);

		if (term != null)
		    term.rollback();
		else
		    ErrorStream.stream(ErrorStream.WARNING).println("AtomicTransaction - cannot abort "+transactionName);

		transactionName = null;
		currentTransactionName = null;
		term = null;
	    }
	}
	catch (Exception e)
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("AtomicTransaction caught: "+e);
	}

	coord = null;
	currControl = null;
	currCoord = null;

	return valid;
    }


private org.omg.CosTransactions.Status getStatus ()

    {

	org.omg.CosTransactions.Status stat = Status.StatusNoTransaction;


	if (_theAction != null)
	{
	    try
	    {
		Coordinator c = _theAction.get_coordinator();

		if (c != null)
		{
		    stat = c.get_status();
		    c = null;
		}
	    }
	    catch (Unavailable e)
	    {
	    }
	    catch (SystemException e)
	    {
		stat = Status.StatusUnknown;
	    }
	}

	return stat;
    }

protected Control _theAction;


private org.omg.CosTransactions.Status _theStatus;


private int _timeout;
    
};
