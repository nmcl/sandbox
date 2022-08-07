/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: TopLevelTransaction.javatmpl,v 1.2.8.2.2.1.22.1.4.1 2000/12/21 11:21:20 nmcl Exp $
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
import org.omg.CosTransactions.*;

import org.omg.CosTransactions.NoTransaction;
import org.omg.CosTransactions.Unavailable;
import org.omg.CosTransactions.InvalidControl;
import org.omg.CosTransactions.HeuristicMixed;
import org.omg.CosTransactions.HeuristicHazard;
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
 * An attempt at a portable way of including the OTS exceptions,
 * which are either mapped as system exceptions or user exceptions,
 * depending upon the Orb.
 */



import org.omg.CosTransactions.WrongTransaction;

import org.omg.CORBA.INVALID_TRANSACTION;
import org.omg.CORBA.TRANSACTION_REQUIRED;
import org.omg.CORBA.TRANSACTION_ROLLEDBACK;





/**
 * Creates a nested top-level transaction.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: TopLevelTransaction.javatmpl,v 1.2.8.2.2.1.22.1.4.1 2000/12/21 11:21:20 nmcl Exp $
 * @since JTS 1.0.
 */

public class TopLevelTransaction extends AtomicTransaction
{

public TopLevelTransaction ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "TopLevelTransaction::TopLevelTransaction ()");
	}

	_originalTransaction = null;
    }

public void finalize ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.DESTRUCTORS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "TopLevelTransaction.finalize ()");
	}

	if (_originalTransaction != null)
	{
	    String name = null;
	    Coordinator coord = null;
	
	    try
	    {
		coord = _originalTransaction.get_coordinator();

		if (coord != null)
		{
		    name = coord.get_transaction_name();
		}
	    }
	    catch (Exception e)
	    {
	    }

	    coord = null;

	    ErrorStream.stream(ErrorStream.WARNING).println("Toplevel transaction going out of scope with nested transaction "+
							    ((name != null) ? name : "UNKNOWN")+" still set.");

	    name = null;
	    _originalTransaction = null;
	}

	super.finalize();
    }

    /**
     * If nested top-level transaction, save current context for resumption
     * later.
     */

public synchronized void begin () throws SystemException, SubtransactionsUnavailable, INVALID_TRANSACTION 
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "TopLevelTransaction::begin ()");
	}

	// already begun?

	if (_originalTransaction != null)
	{
	    throw new INVALID_TRANSACTION ();
	}

	OTS_Current current = OTS.current();

	_originalTransaction = current.suspend();

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "TopLevelTransaction::begin - suspend transaction "+_originalTransaction);
	}

	super.begin();
    }


public synchronized void commit (boolean report_heuristics) throws SystemException, NoTransaction, HeuristicMixed, HeuristicHazard, WrongTransaction // UserException!!

    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "TopLevelTransaction::commit ( "+report_heuristics+" ) called for "+_originalTransaction);
	}

	if (validTransaction())
	{
	    try
	    {
		super.commit(report_heuristics);		
	    }
	    catch (WrongTransaction e1)
	    {
		resumeTransaction();

		throw e1;
	    }
	    catch (SystemException e2)
	    {
		resumeTransaction();

		throw e2;
	    }

	    resumeTransaction();
	}
	else
	    throw new WrongTransaction();
    }


public synchronized void rollback () throws SystemException, NoTransaction, WrongTransaction // UserException!!

    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "TopLevelTransaction::rollback () called for "+_originalTransaction);
	}

	if (validTransaction())
	{
	    try
	    {
		super.rollback();
	    }
	    catch (WrongTransaction e1)
	    {
		resumeTransaction();

		throw e1;
	    }
	    catch (SystemException e2)
	    {
		resumeTransaction();

		throw e2;
	    }

	    resumeTransaction();
	}
	else
	    throw new WrongTransaction();
    }

private final void resumeTransaction ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "TopLevelTransaction::resumeTransaction for "+_originalTransaction);
	}

	try
	{
	    if (_originalTransaction != null)
	    {
		OTS_Current current = OTS.current();

		current.resume(_originalTransaction);

		_originalTransaction = null;
	    }
	}
	catch (Exception e)
	{
	}
    }

private Control _originalTransaction;
 
};
