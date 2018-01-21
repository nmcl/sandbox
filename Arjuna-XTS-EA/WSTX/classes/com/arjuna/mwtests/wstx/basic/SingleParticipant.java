/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: SingleParticipant.java,v 1.1 2002/11/25 11:00:54 nmcl Exp $
 */

package com.arjuna.mwtests.wstx.basic;

import com.arjuna.mwtests.wstx.common.DemoParticipant;

import com.arjuna.mw.wstx.*;

import com.arjuna.mw.wstx.exceptions.NoTransactionException;
import com.arjuna.mw.wstx.exceptions.TransactionRolledBackException;

/**
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: SingleParticipant.java,v 1.1 2002/11/25 11:00:54 nmcl Exp $
 * @since 1.0.
 */

public class SingleParticipant
{

    public static void main (String[] args)
    {
	boolean passed = false;
	
	try
	{
	    UserTransaction ut = UserTransactionFactory.userTransaction();
	    TransactionManager tm = TransactionManagerFactory.transactionManager();
	    DemoParticipant p = new DemoParticipant(ut.identifier());
	    
	    ut.begin();
	    
	    tm.enlist(p);
	    
	    ut.commit();

	    passed = p.passed();
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
