/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: JTAContextImple.java,v 1.4 2003/03/04 12:55:57 nmcl Exp $
 */

package com.arjuna.mwlabs.wscf.model.jta.arjunajta.context.soap;

import com.arjuna.mw.wsas.context.Context;
import com.arjuna.mw.wsas.context.ContextManager;
import com.arjuna.mw.wsas.context.soap.SOAPContext;

import javax.transaction.Transaction;
import javax.transaction.xa.Xid;

import com.hp.mwlabs.ts.jta.transaction.arjunacore.TransactionImple;

import com.arjuna.mw.wscf.utils.DomUtil;

import org.w3c.dom.*;

import javax.xml.parsers.*;

import com.arjuna.mw.wsas.exceptions.SystemException;

/**
 * On demand this class creates the SOAP context information necessary to
 * propagate the hierarchy of coordinators associated with the current
 * thread.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: JTAContextImple.java,v 1.4 2003/03/04 12:55:57 nmcl Exp $
 */

public class JTAContextImple implements SOAPContext
{

    /**
     * Incomplete. Need things like the Coordinator URI!
     */

    public JTAContextImple ()
    {
	_context = null;
    }

    public JTAContextImple (Transaction currentCoordinator)
    {
	_context = null;
	
	initialise(currentCoordinator);
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
	return JTAContextImple.class.getName();
    }

    public String toString ()
    {
	return DomUtil.nodeAsString(_context);
    }

    public void initialise (Object param) throws java.security.InvalidParameterException
    {
	try
	{
	    Transaction currentCoordinator = (Transaction) param;
	    DocumentBuilder builder = DomUtil.getDocumentBuilder();
	    org.w3c.dom.Document doc = builder.newDocument();

	    _context = doc.createElement(_contextName);
	    _context.appendChild(doc.createComment("WARNING: example JTA coordinator hierarchy only!"));

	    if (currentCoordinator != null)
	    {
		org.w3c.dom.Element elem = doc.createElement(_coordName);
		
		elem.appendChild(doc.createTextNode("http://arjuna.com?jta:"+((TransactionImple) currentCoordinator).get_uid().stringForm()));
		
		_context.appendChild(elem);
	    }
	}
	catch (ClassCastException ex)
	{
	    throw new java.security.InvalidParameterException();
	}
    }

    private org.w3c.dom.Element _context;
    
    private static final String _contextName = "jta-context";
    private static final String _coordName = "jta-name";

}
