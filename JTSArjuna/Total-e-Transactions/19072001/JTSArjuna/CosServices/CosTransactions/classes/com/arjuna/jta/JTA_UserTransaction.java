/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: JTA_UserTransaction.javatmpl,v 1.1.14.2.16.2.4.1 2000/12/21 11:21:23 nmcl Exp $
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
import org.omg.CosTransactions.SubtransactionsUnavailable;
import org.omg.CosTransactions.NoTransaction;
import org.omg.CosTransactions.HeuristicMixed;
import org.omg.CosTransactions.HeuristicHazard;
import org.omg.CosTransactions.InvalidControl;
import org.omg.CORBA.NO_PERMISSION;

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
 * An implementation of javax.transaction.UserTransaction.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: JTA_UserTransaction.javatmpl,v 1.1.14.2.16.2.4.1 2000/12/21 11:21:23 nmcl Exp $
 * @since JTS 1.2.4.
 */

public class JTA_UserTransaction extends JTA_BaseTransaction implements javax.transaction.UserTransaction
{
    
public static synchronized javax.transaction.UserTransaction transactionManager ()
    {
	if (JTA_UserTransaction._theUserTransaction == null)
	    JTA_UserTransaction._theUserTransaction = new JTA_UserTransaction();

	return JTA_UserTransaction._theUserTransaction;
    }
    
protected JTA_UserTransaction ()
    {
	super();
    }

private static JTA_UserTransaction _theUserTransaction = null;
    
}
