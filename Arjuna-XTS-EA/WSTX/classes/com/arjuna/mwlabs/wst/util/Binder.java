/*
 * Copyright (C) 2003,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Binder.java,v 1.4 2003/03/24 10:57:02 nmcl Exp $
 */

package com.arjuna.mwlabs.wst.util;

import com.arjuna.mw.wst.*;

import javax.naming.*;

import java.net.URL;

import java.io.FileNotFoundException;

import javax.xml.parsers.*;

/**
 */

// Should probably be done by a JBoss service, for example.

public class Binder
{

    public static String CONFIGURATION_FILE = "/config/wst.xml";
    
    public static void bind () throws NamingException, Exception
    {
	Binder.registerServices(true, CONFIGURATION_FILE);
    }

    public static void bind (String configurationFile) throws NamingException, Exception
    {
	Binder.registerServices(true, configurationFile);
    }

    public static void rebind () throws NamingException, Exception
    {
	Binder.registerServices(false, CONFIGURATION_FILE);
    }

    public static void rebind (String configurationFile) throws NamingException, Exception
    {
	Binder.registerServices(false, configurationFile);
    }
 
    public static void registerServices (boolean bind, String configurationFile) throws NamingException, Exception
    {
	/*
	 * Locate the configuration file.
	 */

	URL url = Binder.class.getResource(configurationFile);

	if (url == null)
	{
	    throw new FileNotFoundException(configurationFile+" not found.");
	}
	else
	{
	    DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
	    DocumentBuilder builder = factory.newDocumentBuilder();
	    org.w3c.dom.Document doc = builder.parse(url.toExternalForm());
	    
	    if (doc == null)
	    {
		throw new FileNotFoundException("Failed to create "+CONFIGURATION_FILE+" doc!");
	    }

	    String userTx = locate(doc.getDocumentElement(), "UserTransaction");
	    String txManager = locate(doc.getDocumentElement(), "TransactionManager");

	    try
	    {
		String initialCtx = System.getProperty(Context.INITIAL_CONTEXT_FACTORY);

		if (initialCtx == null)
		    System.setProperty(Context.INITIAL_CONTEXT_FACTORY, "com.sun.jndi.fscontext.RefFSContextFactory");

		InitialContext ctx = new InitialContext();

		if (bind)
		{
		    ctx.bind(UserTransactionFactory.UserTxName, new javax.naming.Reference(userTx));
		    ctx.bind(TransactionManagerFactory.TxManagerName, new javax.naming.Reference(txManager));
		}
		else
		{
		    ctx.rebind(UserTransactionFactory.UserTxName, new javax.naming.Reference(userTx));
		    ctx.rebind(TransactionManagerFactory.TxManagerName, new javax.naming.Reference(txManager));
		}
	    }
	    catch (NamingException ex)
	    {
		throw ex;
	    }
	    catch (Exception ex)
	    {
		ex.printStackTrace();
	    }
	}
    }

    private static String getImplementation (org.w3c.dom.Node doc)
    {
	org.w3c.dom.NodeList children = doc.getChildNodes();
	    
	for (int i = 0; i < children.getLength(); i++)
	{
	    org.w3c.dom.Node item = children.item(i);
	    
	    if (item.getNodeName().equals("parameter"))
	    {
		org.w3c.dom.Element type = (org.w3c.dom.Element) item;
		    
		if (type.getAttribute("name").equals("className"))
		{
		    return type.getAttribute("value");
		}
	    }
	}
	
	return null;
    }

    private static String locate (org.w3c.dom.Node doc, String name)
    {
	org.w3c.dom.NodeList children = doc.getChildNodes();
	    
	for (int i = 0; i < children.getLength(); i++)
	{
	    org.w3c.dom.Node item = children.item(i);
	    
	    if (item.getNodeName().equals("service"))
	    {
		org.w3c.dom.Element type = (org.w3c.dom.Element) item;
		    
		if (type.getAttribute("name").equals(name))
		{
		    return getImplementation(type);
		}
	    }
	}
	
	return null;
    }

}
