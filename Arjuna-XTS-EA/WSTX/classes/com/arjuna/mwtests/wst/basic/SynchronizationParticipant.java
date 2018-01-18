/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: SynchronizationParticipant.java,v 1.6 2003/03/14 09:55:25 nmcl Exp $
 */

package com.arjuna.mwtests.wst.basic;

import com.arjuna.mw.wst.*;

import com.arjuna.wst.*;

import com.arjuna.mwtests.wst.common.*;

/**
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: SynchronizationParticipant.java,v 1.6 2003/03/14 09:55:25 nmcl Exp $
 * @since 1.0.
 */

public class SynchronizationParticipant
{

    public static void main (String[] args)
    {
	boolean passed = false;
	
	try
	{
	    UserTransaction ut = UserTransactionFactory.userTransaction();
	    TransactionManager tm = TransactionManagerFactory.transactionManager();
	    DemoParticipant p = new DemoParticipant();
	    DemoPhaseZero pz = new DemoPhaseZero();
	    DemoOutcomeNotification on = new DemoOutcomeNotification();
	    
	    ut.begin();
	    
	    tm.enlistForTwoPhase(p, null);
	    tm.enlistForPhaseZero(pz, null);
	    tm.enlistForOutcomeNotification(on, null);
	    
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
