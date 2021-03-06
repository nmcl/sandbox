/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: StartEnd.java,v 1.3 2003/03/14 14:26:34 nmcl Exp $
 */

package com.arjuna.mwtests.wscf.model.twophase;

import com.arjuna.mwlabs.wscf.utils.ProtocolLocator;

import com.arjuna.mw.wscf.UserCoordinator;
import com.arjuna.mw.wscf.UserCoordinatorFactory;

import com.arjuna.mw.wscf.model.twophase.common.*;
import com.arjuna.mw.wscf.model.twophase.outcomes.*;

import com.arjuna.mw.wscf.common.*;

import com.arjuna.mw.wsas.activity.*;

import com.arjuna.mw.wsas.context.Context;
import com.arjuna.mw.wsas.context.DeploymentContext;
import com.arjuna.mw.wsas.context.DeploymentContextFactory;
import com.arjuna.mw.wsas.context.soap.SOAPContext;

import com.arjuna.mw.wsas.exceptions.NoActivityException;

/**
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: StartEnd.java,v 1.3 2003/03/14 14:26:34 nmcl Exp $
 * @since 1.0.
 */

public class StartEnd
{

    public static void main (String[] args)
    {
	boolean passed = false;
	String className = "com.arjuna.mwlabs.wscf.model.twophase.arjunacore.TwoPhaseHLSImple";
	org.w3c.dom.Document implementationDoc = null;
	
	//	System.setProperty("com.arjuna.mw.wscf.protocolImplementation", className);
	
	try
	{
	    ProtocolLocator pl = new ProtocolLocator(className);

	    implementationDoc = pl.getProtocol();
	}
	catch (Exception ex)
	{
	    ex.printStackTrace();
	    
	    System.exit(0);
	}
	
	try
	{
	    UserCoordinator ua = UserCoordinatorFactory.userCoordinator(implementationDoc);
	
	    ua.start();

	    System.out.println("Started: "+ua.activityName()+"\n");

	    Outcome res = ua.end();

	    if (res instanceof CoordinationOutcome)
	    {
		CoordinationOutcome co = (CoordinationOutcome) res;
		
		if (co.result() == TwoPhaseResult.CANCELLED)
		    passed = true;
	    }
	    else
		System.out.println("Result is: "+res);
	}
	catch (NoActivityException ex)
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
