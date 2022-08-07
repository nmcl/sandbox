/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: tx.javatmpl,v 1.1.30.2.4.1 2000/12/21 11:21:21 nmcl Exp $
 */

package com.arjuna.CosTransactions.tx;

import com.arjuna.ArjunaCommon.Template.*;
import com.arjuna.ArjunaCommon.Common.Uid;
import com.arjuna.CosTransactions.*;
import org.omg.CosTransactions.*;
import com.arjuna.ArjunaOTS.*;
import java.util.Hashtable;

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
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: tx.javatmpl,v 1.1.30.2.4.1 2000/12/21 11:21:21 nmcl Exp $
 * @since JTS 1.0.
 */

public class tx
{
    /**
     * These values are pretty arbitrary since we have
     * no way of knowing what they should be. As long as
     * the CPP names are used we should be ok though.
     */

public static final int TX_OK = 0;
public static final int TX_COMMIT_COMPLETED = 1;
public static final int TX_COMMIT_DESICION_LOGGED = 2;
public static final int TX_CHAINED = 3;
public static final int TX_UNCHAINED = 4;
public static final int TX_ERROR = -1;
public static final int TX_FAIL = -2;
public static final int TX_PROTOCOL_ERROR = -3;
public static final int TX_OUTSIDE = -4;
public static final int TX_NO_BEGIN = -5;
public static final int TX_HAZARD = -6;
public static final int TX_HAZARD_NO_BEGIN = -7;
public static final int TX_NOT_SUPPORTED = -8;
public static final int TX_EINVAL = -9;
public static final int TX_ROLLBACK = -10;
public static final int TX_ROLLBACK_NO_BEGIN = -11;
    
public static final synchronized int tx_open ()
    {
	int toReturn = tx.TX_ERROR;  // what to return?
    
	if (!__tx_open)
	{
	    __tx_open = true;
	    toReturn = tx.TX_OK;
	}

	return toReturn;
    }

    /**
     * The X/Open spec. says to raise TX_PROTOCOL_ERROR if called
     * from within a transaction. However, the OTS spec. implies there
     * is no mapping for tx_close. So, do nothing.
     */

public static final synchronized int tx_close ()
    {
	int toReturn = tx.TX_ERROR;
    
	if (__tx_open)
	{
	    __tx_open = false;
	    toReturn = tx.TX_OK;
	}

	return toReturn;
    }

public static final synchronized int tx_allow_nesting ()
    {
	int toReturn = tx.TX_PROTOCOL_ERROR;

	if (!__tx_open)
	{
	    __tx_allow_nesting = true;
	    toReturn = tx.TX_OK;
	}

	return toReturn;
    }

public static final synchronized int tx_begin ()
    {
	int toReturn = tx.TX_OK;
	OTS_Current current = OTS.current();

	if (!__tx_allow_nesting)
	{
	    /*
	     * Already have a transaction?
	     */

	    try
	    {
		Control control = current.get_control();

		if (control != null)
		{
		    /*
		     * Have a transaction already, and not allowed to
		     * create nested transactions!
		     */

		    toReturn = tx.TX_PROTOCOL_ERROR;
		    control = null;
		}
	    }
	    catch (Exception e)
	    {
		// something went wrong!
		
		toReturn = tx.TX_FAIL;
	    }
	}

	if (toReturn == tx.TX_OK)
	{
	    try
	    {
		current.begin();
	    }
	    catch (Exception e)
	    {
		toReturn = tx.TX_FAIL;
	    }
	}
    
	return toReturn;
    }

    /**
     * This needs to implement checked transactions such that only the
     * transaction initiator (thread) can terminate it.
     */

public static final synchronized int tx_rollback ()
    {
	int toReturn = tx.TX_OK;
	OTS_Current current = OTS.current();

	try
	{
	    current.rollback();
	}
	catch (NoTransaction e1)
	{
	    toReturn = tx.TX_NO_BEGIN;
	}
	catch (Exception e2)
	{
	    toReturn = tx.TX_FAIL;
	}

	return toReturn;
    }

public static final synchronized int tx_set_commit_return (int when_return)
    {
	int toReturn = tx.TX_OK;
	boolean b = ((when_return == 0) ? false : true);
	
	if ((when_return == tx.TX_COMMIT_COMPLETED) ||
	    (when_return == tx.TX_COMMIT_DESICION_LOGGED))
	{
	    __tx_report_heuristics.put(Thread.currentThread(), new Boolean(b));
	}
	else
	    toReturn = tx.TX_PROTOCOL_ERROR;

	return toReturn;
    }
    
public static final synchronized int tx_commit ()
    {
	int toReturn = tx.TX_OK;
	OTS_Current current = OTS.current();
	Boolean report_heuristics = (Boolean) __tx_report_heuristics.get(Thread.currentThread());

	if (report_heuristics == null)
	    report_heuristics = new Boolean(true);  // default TRUE
	    
	try
	{
	    boolean when_return = report_heuristics.booleanValue();
		
	    current.commit(when_return);
	}
	catch (NoTransaction e1)
	{
	    toReturn = tx.TX_NO_BEGIN;
	}
	catch (HeuristicMixed e2)
	{
	    toReturn = tx.TX_HAZARD;
	}
	catch (HeuristicHazard e3)
	{
	    toReturn = tx.TX_HAZARD;
	}
	catch (TRANSACTION_ROLLEDBACK  e4)
	{
	    toReturn = tx.TX_ROLLBACK;
	}
	catch (Exception e5)
	{
	    toReturn = tx.TX_FAIL;
	}

	return toReturn;
    }

public static final synchronized int tx_set_transaction_control (int control)
    {
	return tx.TX_FAIL;
    }

public static final synchronized int tx_set_transaction_timeout (int timeout)
    {
	int toReturn = tx.TX_OK;
	OTS_Current current = OTS.current();

	try
	{
	    current.set_timeout(timeout);
	}
	catch (Exception e)
	{
	    toReturn = tx.TX_FAIL;
	}

	return toReturn;
    }
    
private static boolean   __tx_open = false;
private static boolean   __tx_allow_nesting = false;
private static Hashtable __tx_report_heuristics = new Hashtable();
 
};
