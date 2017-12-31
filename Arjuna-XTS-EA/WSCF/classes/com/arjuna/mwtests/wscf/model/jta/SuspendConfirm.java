/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: SuspendConfirm.java,v 1.1 2003/01/07 10:34:00 nmcl Exp $
 */

package com.arjuna.mwtests.wscf.model.jta;

import com.arjuna.mwtests.wscf.common.*;

import com.arjuna.mw.wscf.model.xa.*;

import com.arjuna.mw.wsas.activity.*;

import com.arjuna.mw.wsas.exceptions.NoActivityException;

/**
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: SuspendConfirm.java,v 1.1 2003/01/07 10:34:00 nmcl Exp $
 * @since 1.0.
 */

public class SuspendConfirm
{

    public static void main (String[] args)
    {
	boolean passed = false;
	
	try
	{
	    javax.transaction.TransactionManager tm = TransactionManagerFactory.transactionManager();
	    
	    tm.begin();

	    System.out.println("Started: "+tm.getTransaction()+"\n");

	    javax.transaction.Transaction tx = tm.suspend();
	    
	    System.out.println("Suspended: "+tx+"\n");
	    
	    if (tm.getTransaction() != null)
		System.out.println("Hierarchy still active.");

	    tm.commit();
	}
	catch (IllegalStateException ex)
	{
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
