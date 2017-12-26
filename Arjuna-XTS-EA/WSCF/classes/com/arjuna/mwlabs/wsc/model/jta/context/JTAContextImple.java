/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: JTAContextImple.java,v 1.5 2003/03/04 12:55:56 nmcl Exp $
 */

package com.arjuna.mwlabs.wsc.model.jta.context;

import com.arjuna.mw.wsas.*;
import com.arjuna.mw.wsas.activity.*;

import com.arjuna.mw.wsas.context.Context;
import com.arjuna.mw.wsas.context.ContextManager;
import com.arjuna.mw.wsas.context.soap.SOAPContext;

import com.arjuna.mw.wscf.utils.DomUtil;

import com.hp.mwlabs.ts.jta.transaction.arjunacore.TransactionImple;

import javax.transaction.*;

import org.w3c.dom.*;

import javax.xml.parsers.*;

import com.arjuna.mw.wsas.exceptions.SystemException;

/**
 * On demand this class creates the SOAP context information necessary to
 * propagate the hierarchy of coordinators associated with the current
 * thread.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: JTAContextImple.java,v 1.5 2003/03/04 12:55:56 nmcl Exp $
 */

public class JTAContextImple implements SOAPContext
{

    public JTAContextImple ()
    {
	_context = null;
    }
    
    public JTAContextImple (Transaction currentCoordinator)
    {
	_context = null;
	
	initialise(currentCoordinator);
    }

    public void initialise (Object param) throws java.security.InvalidParameterException
    {
	try
	{
	    Transaction currentCoordinator = (Transaction) param;
	    DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
	    DocumentBuilder builder = null;
	
	    try
	    {
		builder = factory.newDocumentBuilder();
	    }
	    catch (Exception ex)
	    {
		ex.printStackTrace();
	    }
	
	    org.w3c.dom.Document doc = builder.newDocument();

	    _context = doc.createElement(_contextName);

            _context.setAttribute("xmlns:wsu", _wsuNamespace);
            _context.setAttribute("xmlns:wscoor", _wscoorNamespace);
            _context.setAttribute("xmlns:arjuna", _arjunaNamespace);

	    ActivityHierarchy hier = null;
	
	    try
	    {
		hier = UserActivityFactory.userActivity().currentActivity();
	    }
	    catch (SystemException ex)
	    {
		ex.printStackTrace();
	    }

	    if ((currentCoordinator != null) && (hier != null))
	    {
		org.w3c.dom.Element identifier = doc.createElement(_identifier);
		identifier.appendChild(doc.createTextNode(((TransactionImple) currentCoordinator).get_uid().stringForm()));
		
		_context.appendChild(identifier);
	    
		org.w3c.dom.Element expires = doc.createElement(_expires);
		expires.appendChild(doc.createTextNode(""+hier.activity(hier.size()-1).getTimeout()));

		_context.appendChild(expires);

		org.w3c.dom.Element coordinationType = doc.createElement("wscoor:CoordinationType");
		coordinationType.appendChild(doc.createTextNode(_coordinationTypeURI));
	    
		_context.appendChild(coordinationType);
	    }
	}
	catch (ClassCastException ex)
	{
	    throw new java.security.InvalidParameterException();
	}
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

    private org.w3c.dom.Element _context;

    private static final String _wscoorNamespace = "http://schemas.xmlsoap.org/ws/2002/08/wscoor";
    private static final String _wsuNamespace = "http://schemas.xmlsoap.org/ws/2002/07/utility";
    private static final String _arjunaNamespace = "http://arjuna.com/schemas/wsc/2003/01/extension";

    private static final String _contextName = "wscoor:CoordinationContext";
    private static final String _identifier = "wsu:Identifier";
    private static final String _expires = "wsu:Expires";
    private static final String _coordinationType = "CoordinationType";

    public static final String _coordinationTypeURI = "urn:arjuna:jta";

}
