/*
 * Copyright (C) 2003,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Configuration.java,v 1.4 2003/03/24 10:36:43 nmcl Exp $
 */

package com.arjuna.mw.wsas.utils;

import javax.naming.*;

import java.net.URL;

import java.io.FileNotFoundException;

import javax.xml.parsers.*;

/**
 */

public class Configuration
{

    public static final String PROPERTIES = "properties";
    public static final String PROPERTY = "property";
    
    public static final void initialise (String fileName) throws Exception
    {
	/*
	 * Locate the configuration file.
	 */

	URL url = Configuration.class.getResource(fileName);

	if (url == null)
	{
	    throw new FileNotFoundException(fileName+" not found.");
	}
	else
	    initialise(url);
    }

    public static final void initialise (URL url) throws Exception
    {
	DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
	DocumentBuilder builder = factory.newDocumentBuilder();
	org.w3c.dom.Document doc = builder.parse(url.toExternalForm());
	    
	if (doc == null)
	{
	    throw new FileNotFoundException("Failed to create "+url+" doc!");
	}
	
	org.w3c.dom.NodeList children = getProperties(doc.getDocumentElement());

	if (children != null)
	{
	    for (int i = 0; i < children.getLength(); i++)
	    {
		org.w3c.dom.Node item = children.item(i);
	    
		if (item.getNodeName().equals(PROPERTY))
		{
		    org.w3c.dom.Element type = (org.w3c.dom.Element) item;
		    
		    String name = type.getAttribute("name");
		    String value = type.getAttribute("value");
		
		    if ((name != null) && (value != null))
			System.setProperty(name, value);
		}
	    }
	}
    }

    private static final org.w3c.dom.NodeList getProperties (org.w3c.dom.Node elem)
    {
	org.w3c.dom.NodeList children = elem.getChildNodes();

	for (int i = 0; i < children.getLength(); i++)
	{
	    org.w3c.dom.Node item = children.item(i);

	    if (item.getNodeName().equals(PROPERTIES))
	    {
		return item.getChildNodes();
	    }
	    else
	    {
		org.w3c.dom.NodeList descendants = getProperties(item);
		
		if (descendants != null)
		    return descendants;
	    }
	}

	return null;
    }

}
