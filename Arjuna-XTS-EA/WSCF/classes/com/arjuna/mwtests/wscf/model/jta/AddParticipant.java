/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: AddParticipant.java,v 1.1 2003/01/07 10:33:59 nmcl Exp $
 */

package com.arjuna.mwtests.wscf.model.jta;

import com.arjuna.mwtests.wscf.common.*;

import com.arjuna.mw.wscf.model.xa.*;

import com.arjuna.mw.wsas.activity.*;

import com.arjuna.mw.wsas.exceptions.NoActivityException;

/**
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: AddParticipant.java,v 1.1 2003/01/07 10:33:59 nmcl Exp $
 * @since 1.0.
 */

public class AddParticipant
{

    public static void main (String[] args)
    {
	boolean passed = false;
	
	try
	{
	    javax.transaction.TransactionManager tm = TransactionManagerFactory.transactionManager();
	    
	    tm.begin();

	    tm.getTransaction().enlistResource(new DemoXAResource());
	    
	    System.out.println("Started: "+tm+"\n");

	    tm.commit();

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
