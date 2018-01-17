/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: SingleParticipant.java,v 1.6 2003/03/14 09:55:25 nmcl Exp $
 */

package com.arjuna.mwtests.wst.basic;

import com.arjuna.mw.wst.*;

import com.arjuna.wst.*;

import com.arjuna.mwtests.wst.common.DemoParticipant;

/**
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: SingleParticipant.java,v 1.6 2003/03/14 09:55:25 nmcl Exp $
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
	    DemoParticipant p = new DemoParticipant();
	    
	    ut.begin();
	    
	    tm.enlistForTwoPhase(p, null);
	    
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
