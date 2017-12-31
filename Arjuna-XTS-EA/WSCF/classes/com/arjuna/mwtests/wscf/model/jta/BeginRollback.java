/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: BeginRollback.java,v 1.1 2003/01/07 10:33:59 nmcl Exp $
 */

package com.arjuna.mwtests.wscf.model.jta;

import com.arjuna.mw.wscf.model.xa.*;

import javax.transaction.*;
import javax.transaction.xa.*;

import com.arjuna.mw.wscf.common.*;

import com.arjuna.mw.wsas.activity.*;

import com.arjuna.mw.wsas.exceptions.NoActivityException;

/**
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: BeginRollback.java,v 1.1 2003/01/07 10:33:59 nmcl Exp $
 * @since 1.0.
 */

public class BeginRollback
{

    public static void main (String[] args)
    {
	boolean passed = false;
	
	try
	{
	    UserTransaction ut = UserTransactionFactory.userTransaction();
	    
	    ut.begin();

	    System.out.println("Started: "+ut+"\n");

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
