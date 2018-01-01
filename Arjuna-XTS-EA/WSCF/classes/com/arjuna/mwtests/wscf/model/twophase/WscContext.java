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

package com.arjuna.mwtests.wscf.model.twophase;

import com.arjuna.mwlabs.wscf.utils.ProtocolLocator;

import com.arjuna.mw.wscf.model.twophase.api.UserCoordinator;

import com.arjuna.mw.wscf.model.twophase.UserCoordinatorFactory;

import com.arjuna.mw.wscf.model.twophase.common.*;
import com.arjuna.mw.wscf.model.twophase.exceptions.*;

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
	    UserCoordinator ua = UserCoordinatorFactory.userCoordinator();
	    
	    ua.begin();

	    System.out.println("Started: "+ua.identifier()+"\n");

	    DeploymentContext manager = DeploymentContextFactory.deploymentContext();
	    Context theContext = manager.context();

	    System.out.println(com.arjuna.mw.wscf.utils.DomUtil.nodeAsString(((SOAPContext) theContext).context()));
	    
	    ua.cancel();

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
