/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: SuspendNullTransaction.java,v 1.2 2003/02/21 15:12:20 nmcl Exp $
 */

package com.arjuna.mwtests.wst.basic;

import com.arjuna.wst.*;

import com.arjuna.mw.wst.TransactionManager;
import com.arjuna.mw.wst.TransactionManagerFactory;
import com.arjuna.mw.wst.TxContext;

/**
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: SuspendNullTransaction.java,v 1.2 2003/02/21 15:12:20 nmcl Exp $
 * @since 1.0.
 */

public class SuspendNullTransaction
{

    public static void main (String[] args)
    {
	boolean passed = false;
	
	try
	{
	    TransactionManager ut = TransactionManagerFactory.transactionManager();
	
	    TxContext ctx = ut.suspend();

	    System.out.println("Suspended: "+ctx);

	    if (ctx == null)
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
