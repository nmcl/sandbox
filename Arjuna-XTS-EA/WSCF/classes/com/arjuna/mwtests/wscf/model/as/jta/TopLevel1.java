/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: TopLevel1.java,v 1.3 2003/03/14 14:26:34 nmcl Exp $
 */

package com.arjuna.mwtests.wscf.model.as.jta;

import com.arjuna.mwtests.wscf.common.DemoXAParticipant;

import com.arjuna.mwlabs.wscf.utils.ProtocolLocator;

import com.arjuna.mw.wscf.UserCoordinator;
import com.arjuna.mw.wscf.UserCoordinatorFactory;

import com.arjuna.mw.wscf.model.as.CoordinatorManager;
import com.arjuna.mw.wscf.model.as.CoordinatorManagerFactory;

import com.arjuna.mw.wscf.common.*;

import com.arjuna.mw.wscf.model.as.coordinator.xa.outcomes.*;

import com.arjuna.mw.wsas.activity.*;

import com.arjuna.mw.wsas.context.Context;
import com.arjuna.mw.wsas.context.DeploymentContext;
import com.arjuna.mw.wsas.context.DeploymentContextFactory;
import com.arjuna.mw.wsas.context.soap.SOAPContext;

import com.arjuna.mw.wscf.utils.DomUtil;

import com.arjuna.mw.wsas.exceptions.NoActivityException;

import javax.transaction.xa.*;

import javax.xml.parsers.*;

import org.w3c.dom.*;

/**
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: TopLevel1.java,v 1.3 2003/03/14 14:26:34 nmcl Exp $
 * @since 1.0.
 */

public class TopLevel1
{

    public static void main (String[] args)
    {
	boolean passed = false;
	String className = "com.arjuna.mwlabs.wscf.generic.coordinator.jta.JTAHLS";
	org.w3c.dom.Document implementationDoc = null;
	
	System.setProperty("com.arjuna.mw.wscf.protocolImplementation", className);
	
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
	    DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
	    DocumentBuilder builder = factory.newDocumentBuilder();
	    org.w3c.dom.Document doc = builder.newDocument();
	    org.w3c.dom.Element root = doc.createElement("TopLevel1-test");
	
	    ua.start();

	    System.out.println("Started: "+ua.activityName()+"\n");

	    DeploymentContext manager = DeploymentContextFactory.deploymentContext();
	    Context theContext = manager.context();
	    
	    root.appendChild(((SOAPContext) theContext).context());
	    
	    doc.appendChild(root);

	    System.out.println(DomUtil.nodeAsString(doc));

	    CoordinatorManager cm = CoordinatorManagerFactory.coordinatorManager(implementationDoc);
	    
	    //	    cm.addParticipant(new DemoXAParticipant(), Priorities.PARTICIPANT, null);
	    cm.addParticipant(new DemoXAParticipant(), 0, null);

	    System.out.println("\nEnding coordination.");
	    
	    Outcome res = ua.end();

	    if (res instanceof XAOutcome)
	    {
		XAOutcome out = (XAOutcome) res;
		
		if (out.data() == null)
		    passed = true;
		else
		    System.out.println("Result is: "+((XAException) out.data()));
	    }
	    else
		System.out.println("Outcome is: "+res);
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
