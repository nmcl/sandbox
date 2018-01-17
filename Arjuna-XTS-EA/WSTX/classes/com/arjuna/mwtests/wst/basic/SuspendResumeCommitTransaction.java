/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: SuspendResumeCommitTransaction.java,v 1.2 2003/02/21 15:12:20 nmcl Exp $
 */

package com.arjuna.mwtests.wst.basic;

import com.arjuna.wst.*;

import com.arjuna.mw.wst.UserTransaction;
import com.arjuna.mw.wst.UserTransactionFactory;
import com.arjuna.mw.wst.TransactionManager;
import com.arjuna.mw.wst.TransactionManagerFactory;
import com.arjuna.mw.wst.TxContext;

/**
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: SuspendResumeCommitTransaction.java,v 1.2 2003/02/21 15:12:20 nmcl Exp $
 * @since 1.0.
 */

public class SuspendResumeCommitTransaction
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
	    
	    tm.resume(ctx);

	    System.out.println("\nResumed");
	    
	    ut.commit();

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
