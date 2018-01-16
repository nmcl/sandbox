/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: NullRollbackTransaction.java,v 1.3 2003/02/13 11:45:55 nmcl Exp $
 */

package com.arjuna.mwtests.wst.basic;

import com.arjuna.mw.wst.UserTransaction;
import com.arjuna.mw.wst.UserTransactionFactory;

/**
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: NullRollbackTransaction.java,v 1.3 2003/02/13 11:45:55 nmcl Exp $
 * @since 1.0.
 */

public class NullRollbackTransaction
{

    public static void main (String[] args)
    {
	boolean passed = false;
	
	try
	{
	    UserTransaction ut = UserTransactionFactory.userTransaction();
	
	    ut.begin();
	    
	    ut.rollback();

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
