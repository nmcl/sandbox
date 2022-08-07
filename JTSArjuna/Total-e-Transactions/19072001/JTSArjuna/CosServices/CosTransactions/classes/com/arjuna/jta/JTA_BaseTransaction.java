/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: JTA_BaseTransaction.javatmpl,v 1.1.2.1.2.2.2.1.10.2.20.1 2001/06/15 12:16:20 nmcl Exp $
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




package com.arjuna.jta;

import org.omg.CosTransactions.*;
import com.arjuna.CosTransactions.*;
import javax.transaction.*;
import com.arjuna.ArjunaCommon.Common.*;

import javax.transaction.NotSupportedException;
import javax.transaction.RollbackException;
import javax.transaction.HeuristicMixedException;
import javax.transaction.HeuristicRollbackException;
import javax.transaction.HeuristicMixedException;
import javax.transaction.SystemException;
import java.lang.SecurityException;
import java.lang.IllegalStateException;
import java.lang.NullPointerException;
import org.omg.CosTransactions.SubtransactionsUnavailable;
import org.omg.CosTransactions.NoTransaction;
import org.omg.CosTransactions.HeuristicMixed;
import org.omg.CosTransactions.HeuristicHazard;
import org.omg.CosTransactions.InvalidControl;
import org.omg.CORBA.NO_PERMISSION;
import org.omg.CORBA.BAD_OPERATION;

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




/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: OTS_Exceptions.h,v 1.1.42.1 2001/03/20 16:24:47 nmcl Exp $
 */




/*
 * The Minor version numbers for some system exceptions
 * we may raise.
 */




/*
 * UNKNOWN
 */







/*
 * BAD_OPERATION
 */









/*
 * BAD_PARAM
 */




/*
 * TRANSACTION_ROLLEDBACK
 */






/*
 * INVALID_TRANSACTION
 */









/*
 * WRONG_TRANSACTION
 */





/*
 * TRANSACTION_REQUIRED
 */







/**
 * Some common methods for UserTransaction and TransactionManager.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: JTA_BaseTransaction.javatmpl,v 1.1.2.1.2.2.2.1.10.2.20.1 2001/06/15 12:16:20 nmcl Exp $
 * @since JTS 2.1.
 */

public class JTA_BaseTransaction
{

public void begin () throws javax.transaction.NotSupportedException, javax.transaction.SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JTA, "JTA_BaseTransaction.begin");
	}

	/*
	 * We can supported subtransactions, so should have the option
	 * to let programmer use them. Strict conformance will always
	 * say no.
	 */

	if (!JTA_BaseTransaction._supportSubtransactions)
	{
	    try
	    {
		checkTransactionState();
	    }
	    catch (IllegalStateException e1)
	    {
		throw new NotSupportedException();
	    }
	    catch (org.omg.CORBA.SystemException e2)
	    {
		throw new javax.transaction.SystemException(e2.toString());
	    }
	}

	try
	{
	    OTS.current().begin();
	}
	catch (org.omg.CosTransactions.SubtransactionsUnavailable e3)
	{
	    // shouldn't happen if we get here from the previous checks!

	    throw new NotSupportedException();
	}
	catch (org.omg.CORBA.SystemException e4)
	{
	    throw new javax.transaction.SystemException(e4.toString());
	}
    }

    /**
     * We will never throw a HeuristicRollbackException because if we
     * get a HeuristicRollback from a resource, and can successfully
     * rollback the other resources, this is then the same as having
     * simply been forced to rollback the transaction during phase 1.
     * The OTS interfaces do not allow a differentiation.
     */
    
public void commit () throws javax.transaction.RollbackException,
	                     javax.transaction.HeuristicMixedException,
	                     javax.transaction.HeuristicRollbackException, java.lang.SecurityException,
                             java.lang.IllegalStateException, javax.transaction.SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JTA, "JTA_BaseTransaction.commit");
	}

	try
	{
	    OTS.current().commit(true);
	}
	catch (NullPointerException ex)
	{
	    throw new IllegalStateException("JTA_BaseTransaction.commit - no transaction!");
	}
	catch (org.omg.CosTransactions.NoTransaction e1)
	{
	    throw new IllegalStateException("JTA_BaseTransaction.commit - no transaction!");
	}
	catch (org.omg.CosTransactions.HeuristicMixed e2)
	{
	    throw new javax.transaction.HeuristicMixedException("JTA_HeuristicType.HEURISTIC_MIXED");
	}
	catch (org.omg.CosTransactions.HeuristicHazard e3)
	{
	    throw new javax.transaction.HeuristicMixedException("JTA_HeuristicType.HEURISTIC_HAZARD");
	}
	catch (TRANSACTION_ROLLEDBACK  e4)
	{
	    throw new RollbackException();
	}
	catch (org.omg.CORBA.NO_PERMISSION e5)
	{
	    throw new SecurityException();
	}
	catch (INVALID_TRANSACTION  e6)
	{
	    throw new IllegalStateException("JTA_BaseTransaction.commit - no transaction!");
	}
	catch (BAD_OPERATION e7)
	{
	    throw new IllegalStateException("JTA_BaseTransaction.commit - no transaction!");
	}
	catch (org.omg.CORBA.SystemException e8)
	{
	    throw new javax.transaction.SystemException(e8.toString());
	}

	checkTransactionState();
    }

public void rollback () throws java.lang.IllegalStateException, java.lang.SecurityException, javax.transaction.SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JTA, "JTA_BaseTransaction.rollback");
	}

	try
	{
	    OTS.current().rollback();
	}
	catch (NullPointerException ex)
	{
	    throw new IllegalStateException();
	}	    
	catch (org.omg.CosTransactions.NoTransaction e1)
	{
	    throw new IllegalStateException();
	}
	catch (org.omg.CORBA.NO_PERMISSION e2)
	{
	    throw new SecurityException();
	}
	catch (INVALID_TRANSACTION  e3)
	{
	    throw new IllegalStateException("JTA_BaseTransaction.commit - no transaction!");
	}
	catch (BAD_OPERATION e4)
	{
	    throw new IllegalStateException("JTA_BaseTransaction.commit - no transaction!");
	}
	catch (org.omg.CORBA.SystemException e5)
	{
	    throw new javax.transaction.SystemException(e5.toString());
	}

	checkTransactionState();
    }

public void setRollbackOnly () throws java.lang.IllegalStateException, javax.transaction.SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JTA, "JTA_BaseTransaction.setRollbackOnly");
	}

	try
	{
	    OTS.current().rollback_only();
	}
	catch (NullPointerException ex)
	{
	    throw new IllegalStateException("No such transaction!");
	}	    
	catch (org.omg.CosTransactions.NoTransaction e1)
	{
	    throw new IllegalStateException("No such transaction!");
	}
	catch (org.omg.CORBA.SystemException e2)
	{
	    throw new javax.transaction.SystemException(e2.toString());
	}
    }

public int getStatus () throws javax.transaction.SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JTA, "JTA_BaseTransaction.getStatus");
	}

	try
	{
	    return JTA_Utility.convertStatus(OTS.current().get_status());
	}
	catch (NullPointerException ex)
	{
	    return javax.transaction.Status.STATUS_NO_TRANSACTION;
	}
	catch (Exception e)
	{
	    throw new javax.transaction.SystemException(e.toString());
	}
    }

public void setTransactionTimeout (int seconds) throws javax.transaction.SystemException
    {
	try
	{
	    OTS.current().set_timeout(seconds);
	}
	catch (Exception e)
	{
	    throw new javax.transaction.SystemException(e.toString());
	}
    }

public int getTimeout () throws javax.transaction.SystemException
    {
	try
	{
	    return OTS.current().get_timeout();
	}
	catch (Exception e)
	{
	    throw new javax.transaction.SystemException(e.toString());
	}
    }
 
protected JTA_BaseTransaction ()
    {
    }

    /**
     * Called when we want to make sure this thread does not
     * already have a transaction associated with it.
     */
    
final void checkTransactionState () throws IllegalStateException, javax.transaction.SystemException
    {
	try
	{
	    Control cont = OTS.current().get_control();

	    /*
	     * Control may not be null, but its coordinator may be.
	     */
	    
	    if (cont != null)
	    {
		Coordinator coord = cont.get_coordinator();

		if (coord != null)
		{
		    if ((coord.get_status() == org.omg.CosTransactions.Status.StatusActive) && (!_supportSubtransactions))
		    {
			throw new IllegalStateException("JTA_BaseTransaction.checkTransactionState - already associated with transaction.");
		    }
		}
	    
		cont = null;
	    }
	}	    
	catch (org.omg.CORBA.SystemException e1)
	{
	    throw new javax.transaction.SystemException(e1.toString());
	}
	catch (org.omg.CosTransactions.Unavailable e2)
	{
	    // ok, no transaction currently associated with thread.
	}
	catch (NullPointerException ex)
	{
	    // ok, no transaction currently associated with thread.
	}
    }

private static boolean _supportSubtransactions = false;
    
    static
    {
	String subtran = PropertyManager.getProperty(JTA_Environment.JTA_SUPPORT_SUBTRANSACTIONS);
	
	if ((subtran != null) && (subtran.equals("YES")))
	    _supportSubtransactions = true;
    }
    
}
