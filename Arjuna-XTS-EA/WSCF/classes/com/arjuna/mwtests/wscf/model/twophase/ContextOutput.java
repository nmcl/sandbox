/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ContextOutput.java,v 1.2 2003/01/17 15:39:35 nmcl Exp $
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

import javax.xml.parsers.*;

/**
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: ContextOutput.java,v 1.2 2003/01/17 15:39:35 nmcl Exp $
 * @since 1.0.
 */

public class ContextOutput
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
	    DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
	    DocumentBuilder builder = factory.newDocumentBuilder();
	    org.w3c.dom.Document doc = builder.newDocument();
	    org.w3c.dom.Element root = doc.createElement("Context-test");

	    root.appendChild(((SOAPContext) theContext).context());
	    doc.appendChild(root);

	    System.out.println(com.arjuna.mw.wscf.utils.DomUtil.nodeAsString(doc));
	    
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
