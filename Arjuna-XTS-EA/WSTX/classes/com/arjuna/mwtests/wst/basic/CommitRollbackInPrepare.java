/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: CommitRollbackInPrepare.java,v 1.5 2003/03/14 09:55:24 nmcl Exp $
 */

package com.arjuna.mwtests.wst.basic;

import com.arjuna.mw.wst.*;

import com.arjuna.wst.*;

import com.arjuna.mwtests.wst.common.*;

/**
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: CommitRollbackInPrepare.java,v 1.5 2003/03/14 09:55:24 nmcl Exp $
 * @since 1.0.
 */

public class CommitRollbackInPrepare
{

    public static void main (String[] args)
    {
	boolean passed = false;
	
	try
	{
	    UserTransaction ut = UserTransactionFactory.userTransaction();
	    TransactionManager tm = TransactionManagerFactory.transactionManager();
	    FailureParticipant p1 = new FailureParticipant(FailureParticipant.FAIL_IN_PREPARE, FailureParticipant.NONE);
	    DemoParticipant p2 = new DemoParticipant();
	    
	    ut.begin();
	    
	    tm.enlistForTwoPhase(p1, null);
	    tm.enlistForTwoPhase(p2, null);
	    
	    ut.commit();

	    passed = false;
	}
	catch (com.arjuna.wst.TransactionRolledBackException ex)
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
