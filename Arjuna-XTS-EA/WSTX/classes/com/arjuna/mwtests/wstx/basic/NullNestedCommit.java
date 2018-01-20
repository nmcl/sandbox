/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: NullNestedCommit.java,v 1.1 2002/11/25 11:00:54 nmcl Exp $
 */

package com.arjuna.mwtests.wstx.basic;

import com.arjuna.mw.wstx.UserTransaction;
import com.arjuna.mw.wstx.UserTransactionFactory;

import com.arjuna.mw.wstx.exceptions.NoTransactionException;
import com.arjuna.mw.wstx.exceptions.TransactionRolledBackException;

/**
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: NullNestedCommit.java,v 1.1 2002/11/25 11:00:54 nmcl Exp $
 * @since 1.0.
 */

public class NullNestedCommit
{

    public static void main (String[] args)
    {
	boolean passed = false;
	
	try
	{
	    UserTransaction ut = UserTransactionFactory.userTransaction();
	
	    ut.begin();
	    
	    ut.begin();
	    
	    ut.commit();

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
