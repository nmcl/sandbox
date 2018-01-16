/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: MultiParticipants.java,v 1.5 2003/03/14 09:55:24 nmcl Exp $
 */

package com.arjuna.mwtests.wst.basic;

import com.arjuna.mw.wst.*;

import com.arjuna.wst.*;

import com.arjuna.mwtests.wst.common.DemoParticipant;

/**
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: MultiParticipants.java,v 1.5 2003/03/14 09:55:24 nmcl Exp $
 * @since 1.0.
 */

public class MultiParticipants
{

    public static void main (String[] args)
    {
	boolean passed = false;
	
	try
	{
	    UserTransaction ut = UserTransactionFactory.userTransaction();
	    TransactionManager tm = TransactionManagerFactory.transactionManager();
	    DemoParticipant p1 = new DemoParticipant();
	    DemoParticipant p2 = new DemoParticipant();
	    DemoParticipant p3 = new DemoParticipant();
	    DemoParticipant p4 = new DemoParticipant();
	    
	    ut.begin();
	    
	    tm.enlistForTwoPhase(p1, null);
	    tm.enlistForTwoPhase(p2, null);
	    tm.enlistForTwoPhase(p3, null);
	    tm.enlistForTwoPhase(p4, null);
	    
	    ut.commit();

	    passed = p1.passed() && p2.passed() && p3.passed() && p4.passed();
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