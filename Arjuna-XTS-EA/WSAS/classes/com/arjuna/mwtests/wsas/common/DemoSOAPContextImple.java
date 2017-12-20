/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: DemoSOAPContextImple.java,v 1.4 2003/02/13 11:26:11 nmcl Exp $
 */

package com.arjuna.mwtests.wsas.common;

import com.arjuna.mw.wsas.UserActivity;
import com.arjuna.mw.wsas.UserActivityFactory;

import com.arjuna.mw.wsas.activity.ActivityHierarchy;

import com.arjuna.mw.wsas.context.Context;
import com.arjuna.mw.wsas.context.ContextManager;
import com.arjuna.mw.wsas.context.soap.SOAPContext;

import org.w3c.dom.*;

import org.apache.xml.serialize.DOMWriterImpl;

import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.DocumentBuilder;

import com.arjuna.mw.wsas.exceptions.SystemException;

/**
 */

public class DemoSOAPContextImple implements SOAPContext
{

    public DemoSOAPContextImple (String id)
    {
	try
	{
	    DocumentBuilderFactory docFactory = DocumentBuilderFactory.newInstance();
	    DocumentBuilder docBuilder = docFactory.newDocumentBuilder();
	
	    org.w3c.dom.Document doc = docBuilder.newDocument();

	    _context = doc.createElement(id);

	    _context.appendChild(doc.createTextNode("urn:mycomputer.org:"+id+":foo:bar"));
	}
	catch (Exception ex)
	{
	    ex.printStackTrace();
	    
	    _context = null;
	}
    }

    public void initialise (Object param) throws java.security.InvalidParameterException
    {
    }
    
    /**
     * @return the context document object.
     */

    public org.w3c.dom.Element context ()
    {
	return _context;
    }

    /**
     * @return <code>true</code> if the context data goes in the header,
     * <code>false</code> otherwise.
     */

    public boolean header ()
    {
	return true;
    }
    
    public int size ()
    {
	DOMWriterImpl domWriter = new DOMWriterImpl(false);
	String data = domWriter.writeToString(_context);

	return data.length();
    }
   
    public boolean valid ()
    {
	return true;
    }

    public byte[] contextAsByteArray ()
    {
	DOMWriterImpl domWriter = new DOMWriterImpl(false);
	String data = domWriter.writeToString(_context);
	    
	return data.getBytes();
    }

    public String identifier ()
    {
	return "DemoSOAPContextImple";
    }

    public String toString ()
    {
	DOMWriterImpl domWriter = new DOMWriterImpl(false);

	return domWriter.writeToString(_context);
    }
    
    private org.w3c.dom.Element _context;
    
}
