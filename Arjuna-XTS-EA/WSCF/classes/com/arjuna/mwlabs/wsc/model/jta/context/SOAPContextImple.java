/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: SOAPContextImple.java,v 1.4 2003/03/04 12:55:57 nmcl Exp $
 */

package com.arjuna.mwlabs.wsc.model.jta.context;

import com.arjuna.mw.wsas.context.Context;
import com.arjuna.mw.wsas.context.ContextManager;
import com.arjuna.mw.wsas.context.soap.SOAPContext;

import com.arjuna.mw.wscf.utils.DomUtil;

import org.w3c.dom.*;

import com.arjuna.mw.wsas.exceptions.SystemException;

/**
 */

public class SOAPContextImple implements SOAPContext
{

    public SOAPContextImple ()
    {
	ContextManager cxman = new ContextManager();
	Context[] contexts = cxman.contexts();

	for (int i = 0; i < contexts.length; i++)
	{
	    if (contexts[i].identifier().equals(com.arjuna.mwlabs.wsc.model.jta.context.JTAContextImple.class.getName()))
	    {
		if (contexts[i] instanceof SOAPContext)
		{
		    _context = ((SOAPContext) contexts[i]).context();
		}
		else
		    System.err.println("SOAPContextImple ignoring: "+contexts[i]);
		break;
	    }
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
	return DomUtil.nodeAsString(_context).length();
    }
   
    public boolean valid ()
    {
	return true;
    }

    public byte[] contextAsByteArray ()
    {
	return DomUtil.nodeAsString(_context).getBytes();
    }

    public String identifier ()
    {
	return SOAPContextImple.class.getName();
    }
    
    public String toString ()
    {
	return DomUtil.nodeAsString(_context);
    }
    
    private org.w3c.dom.Element _context;

}
