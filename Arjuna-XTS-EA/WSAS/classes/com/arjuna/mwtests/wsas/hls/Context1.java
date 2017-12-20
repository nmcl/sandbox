/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Context1.java,v 1.2 2003/01/17 15:38:07 nmcl Exp $
 */

package com.arjuna.mwtests.wsas.hls;

import com.arjuna.mw.wsas.UserActivity;
import com.arjuna.mw.wsas.UserActivityFactory;
import com.arjuna.mw.wsas.ActivityManagerFactory;
import com.arjuna.mw.wsas.ActivityManager;

import com.arjuna.mw.wsas.context.Context;
import com.arjuna.mw.wsas.context.DeploymentContext;
import com.arjuna.mw.wsas.context.DeploymentContextFactory;
import com.arjuna.mw.wsas.context.soap.SOAPContext;

import com.arjuna.mw.wsas.activity.ActivityHierarchy;

import com.arjuna.mw.wsas.exceptions.NoActivityException;

import com.arjuna.mwtests.wsas.common.DemoHLS;

import org.w3c.dom.*;

import org.apache.xml.serialize.DOMWriterImpl;

import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.DocumentBuilder;

/**
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: Context1.java,v 1.2 2003/01/17 15:38:07 nmcl Exp $
 * @since 1.0.
 */

public class Context1
{

    public static void main (String[] args)
    {
	boolean passed = false;
	
	try
	{
	    UserActivity ua = UserActivityFactory.userActivity();
	    ActivityManagerFactory.activityManager().addHLS(new DemoHLS());
	    DocumentBuilderFactory docFactory = DocumentBuilderFactory.newInstance();
	    DocumentBuilder docBuilder = docFactory.newDocumentBuilder();
	    org.w3c.dom.Document factory = docBuilder.newDocument();
	    org.w3c.dom.Element root = factory.createElement("Context1-test");
	    
	    ua.start();
	    
	    System.out.println("Started: "+ua.activityName());

	    ua.start();
	    
	    System.out.println("Started: "+ua.activityName()+"\n");

	    DeploymentContext manager = DeploymentContextFactory.deploymentContext();
	    Context theContext = manager.context();

	    root.appendChild(((SOAPContext) theContext).context());
	    
	    org.w3c.dom.Document doc = docBuilder.newDocument();
	    doc.appendChild(root);
	    
	    DOMWriterImpl domWriter = new DOMWriterImpl(false);

	    System.out.println(domWriter.writeToString(doc));

	    ua.end();

	    System.out.println("\nFinished child activity.\n");

	    theContext = manager.context();

	    root = factory.createElement("Context1-test");

	    root.appendChild(((SOAPContext) theContext).context());
	    
	    doc = docBuilder.newDocument();
	    doc.appendChild(root);

	    System.out.println(domWriter.writeToString(doc));

	    ua.end();

	    System.out.println("\nFinished parent activity.\n");

	    theContext = manager.context();

	    root = factory.createElement("Context1-test");

	    root.appendChild(((SOAPContext) theContext).context());
	    
	    doc = docBuilder.newDocument();
	    doc.appendChild(root);

	    System.out.println(domWriter.writeToString(doc));

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
