/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: SchemaName.java,v 1.2 2003/01/17 15:39:35 nmcl Exp $
 */

package com.arjuna.mwtests.wscf.xml;

import com.arjuna.mw.wscf.UserCoordinator;
import com.arjuna.mw.wscf.UserCoordinatorFactory;

import com.arjuna.mw.wsas.exceptions.NoActivityException;

import com.arjuna.mw.wscf.exceptions.ProtocolNotRegisteredException;

import java.net.URL;

import javax.xml.parsers.*;

/**
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: SchemaName.java,v 1.2 2003/01/17 15:39:35 nmcl Exp $
 * @since 1.0.
 */

public class SchemaName
{

    public static void main (String[] args)
    {
	boolean passed = false;
	
	try
	{
	    URL url = ClassLoader.getSystemClassLoader().getResource("com/arjuna/mwtests/wscf/xml/example.xml");

	    if (url == null)
	    {
		System.err.println("No example.xml found!");
	    }
	    else
	    {
		DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
		DocumentBuilder builder = factory.newDocumentBuilder();
		org.w3c.dom.Document doc = builder.parse(url.toExternalForm());
	    
		if (doc == null)
		{
		    System.err.println("Failed to create example doc!");
		}

		UserCoordinator ua = UserCoordinatorFactory.userCoordinator(doc);
	
		ua.start();
		
		ua.end();
	    }
	}
	catch (ProtocolNotRegisteredException ex)
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
