/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: SOAPContextImple.java,v 1.4 2003/02/13 11:26:10 nmcl Exp $
 */

package com.arjuna.mwlabs.wsas.context.soap;

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

public class SOAPContextImple implements SOAPContext
{

    public SOAPContextImple ()
    {
	this("");
    }

    public SOAPContextImple (String id)
    {
	try
	{
	    DocumentBuilderFactory docFactory = DocumentBuilderFactory.newInstance();
	    DocumentBuilder docBuilder = docFactory.newDocumentBuilder();
	
	    _doc = docBuilder.newDocument();
	    
	    _context = _doc.createElement(_contextName);

	    if (addBasicContext(_context))
	    {
		ContextManager cxman = new ContextManager();
		Context[] contexts = cxman.contexts();

		org.w3c.dom.Element root = _doc.createElement(_hlsContext);
	
		if (contexts != null)
		{
		    for (int i = 0; i < contexts.length; i++)
		    {
			if (contexts[i] instanceof SOAPContext)
			{
			    root.appendChild(((SOAPContext) contexts[i]).context());
			}
			else
			    System.err.println("SOAPContextImple ignoring: "+contexts[i]);
		    }
		}

		_context.appendChild(root);
	    }
	}
	catch (Exception ex)
	{
	    ex.printStackTrace();
	    
	    _context = null;
	    _doc = null;
	}
    }
    
    public void initialise (Object param)
    {
    }
    
    /**
     * @return the context document object.
     */

    public org.w3c.dom.Element context () throws java.security.InvalidParameterException
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
	return "SOAPContextImple";
    }

    public String toString ()
    {
	DOMWriterImpl domWriter = new DOMWriterImpl(false);

	return domWriter.writeToString(_context);
    }

    /**
     * @return <code>true</code> if a context was added, <code>false</code>
     * otherwise.
     */

    private final boolean addBasicContext (org.w3c.dom.Element root)
    {
	ActivityHierarchy hier = null;
	boolean added = false;
	
	try
	{
	    hier = UserActivityFactory.userActivity().currentActivity();
	}
	catch (SystemException ex)
	{
	    ex.printStackTrace();
	}

	if (hier != null)
	{
	    added = true;
	    
	    for (int i = 0; i < hier.size(); i++)
	    {
		org.w3c.dom.Element element = _doc.createElement(_contextElement);
		org.w3c.dom.Element timeout = _doc.createElement("timeout");

		timeout.appendChild(_doc.createTextNode(""+hier.activity(i).getTimeout()));
		element.appendChild(timeout);

		org.w3c.dom.Element type = _doc.createElement("type");
		type.appendChild(_doc.createTextNode(_typeName));
		element.appendChild(type);

		org.w3c.dom.Element ctxId = _doc.createElement("ctxId");
		ctxId.appendChild(_doc.createTextNode("urn:"+hier.activity(i).toString()));
		element.appendChild(ctxId);

		root.appendChild(element);
	    }
	}
	
	return added;
    }
    
    private org.w3c.dom.Element  _context;
    private org.w3c.dom.Document _doc;
    
    private static final String _typeName = "ArjunaTechnologies";
    private static final String _contextName = "context";
    private static final String _contextElement = "context-entry";
    private static final String _hlsContext = "extended-context-entry";

}
