/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: SuspendTransaction.java,v 1.2 2003/02/21 15:12:20 nmcl Exp $
 */

package com.arjuna.mwtests.wst.basic;

import com.arjuna.mw.wst.*;

/**
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: SuspendTransaction.java,v 1.2 2003/02/21 15:12:20 nmcl Exp $
 * @since 1.0.
 */

public class SuspendTransaction
{

    public static void main (String[] args)
    {
	boolean passed = false;
	
	try
	{
	    UserTransaction ut = UserTransactionFactory.userTransaction();
	    TransactionManager tm = TransactionManagerFactory.transactionManager();
	
	    ut.begin();
	    
	    TxContext ctx = tm.suspend();

	    System.out.println("Suspended: "+ctx);
	    
	    passed = true;
	}
	catch (Exception ex)
	{
	    ex.printStackTrace();
	}
	
	if (passed)
	    System.out.println("\nPassed.");
	else
	    System.out.println("\nFailed.");
    }

}
