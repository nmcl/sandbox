/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: WscContext.java,v 1.3 2003/02/13 11:28:42 nmcl Exp $
 */

package com.arjuna.mwtests.wscf.model.jta;

import com.arjuna.mw.wscf.model.xa.*;

import javax.transaction.*;

import com.arjuna.mw.wscf.common.*;

import com.arjuna.mw.wsas.activity.*;

import com.arjuna.mw.wsas.context.Context;
import com.arjuna.mw.wsas.context.DeploymentContext;
import com.arjuna.mw.wsas.context.DeploymentContextFactory;
import com.arjuna.mw.wsas.context.soap.SOAPContext;

import com.arjuna.mw.wsas.exceptions.NoActivityException;

import com.arjuna.mw.wscf.exceptions.*;

/**
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: WscContext.java,v 1.3 2003/02/13 11:28:42 nmcl Exp $
 * @since 1.0.
 */

public class WscContext
{

    public static void main (String[] args)
    {
	boolean passed = false;
	
	try
	{
	    UserTransaction ut = UserTransactionFactory.userTransaction();
	    
	    ut.begin();

	    System.out.println("Started: "+ut+"\n");

	    DeploymentContext manager = DeploymentContextFactory.deploymentContext();
	    Context theContext = manager.context();

	    System.out.println(com.arjuna.mw.wscf.utils.DomUtil.nodeAsString(((SOAPContext) theContext).context()));
	    
	    ut.rollback();

	    passed = true;
	}
	catch (Exception ex)
	{
	    ex.printStackTrace();

	    passed = false;
	}
	
	if (passed)
	    System.out.println("\nPassed.");
	else
	    System.out.println("\nFailed.");
    }

}
