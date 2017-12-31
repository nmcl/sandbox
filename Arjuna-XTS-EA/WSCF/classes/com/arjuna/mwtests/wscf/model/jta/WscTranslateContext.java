/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: WscTranslateContext.java,v 1.4 2003/03/04 12:55:57 nmcl Exp $
 */

package com.arjuna.mwtests.wscf.model.jta;

import com.arjuna.mw.wscf.model.xa.*;

import com.arjuna.mw.wscf.common.*;

import com.arjuna.mw.wsas.activity.*;

import javax.transaction.*;

import com.arjuna.mw.wsas.context.Context;
import com.arjuna.mw.wsas.context.DeploymentContext;
import com.arjuna.mw.wsas.context.DeploymentContextFactory;
import com.arjuna.mw.wsas.context.soap.SOAPContext;

import com.arjuna.mw.wsas.exceptions.NoActivityException;

import com.arjuna.mw.wscf.exceptions.*;

import com.arjuna.mw.wscf.utils.DomUtil;

import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.DocumentBuilder;

import org.w3c.dom.Element;

/**
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: WscTranslateContext.java,v 1.4 2003/03/04 12:55:57 nmcl Exp $
 * @since 1.0.
 */

public class WscTranslateContext
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
	    SOAPContext theContext = (SOAPContext) manager.context();

	    System.out.println(com.arjuna.mw.wscf.utils.DomUtil.nodeAsString(theContext.context()));

	    org.w3c.dom.Element wscCtx = translate(theContext.context());
	    
	    System.out.println("\nNow got "+DomUtil.nodeAsString(wscCtx));

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

    static private org.w3c.dom.Element translate (org.w3c.dom.Element ctx)
    {
	try
	{
	    org.w3c.dom.Document doc = ctx.getOwnerDocument();
	    
	    org.w3c.dom.Element regServiceElement = doc.createElement("wscoor:RegistrationService");
	    org.w3c.dom.Element regAddressElement = doc.createElement("wsu:Address");

	    regAddressElement.appendChild(doc.createTextNode("htpp://www.arjuna.com?dummyRegistrationServiceAddress"));
	
	    regServiceElement.appendChild(regAddressElement);
	
	    ctx.appendChild(regServiceElement);

	    return ctx;
	}
	catch (Exception ex)
	{
	    // TODO deal with correctly!

	    ex.printStackTrace();
	}
	
	return null;
    }

    private static final String _wscoorNamespace = "http://schemas.xmlsoap.org/ws/2002/08/wscoor";
    private static final String _wsuNamespace = "http://schemas.xmlsoap.org/ws/2002/07/utility";
    private static final String _arjunaNamespace = "http://arjuna.com/schemas/wsc/2003/01/extension";
	
}
