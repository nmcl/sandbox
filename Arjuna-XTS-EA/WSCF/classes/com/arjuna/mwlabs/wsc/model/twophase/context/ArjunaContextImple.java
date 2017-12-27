/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ArjunaContextImple.java,v 1.5 2003/03/04 12:55:57 nmcl Exp $
 */

package com.arjuna.mwlabs.wsc.model.twophase.context;

import com.arjuna.mw.wscf.utils.*;

import com.arjuna.mwlabs.wscf.model.twophase.arjunacore.ACCoordinator;

import com.hp.mw.ts.arjuna.coordinator.TwoPhaseCoordinator;
import com.hp.mw.ts.arjuna.coordinator.ActionHierarchy;

import com.arjuna.mw.wsas.context.Context;
import com.arjuna.mw.wsas.context.ContextManager;
import com.arjuna.mw.wsas.context.soap.SOAPContext;

import com.arjuna.mw.wsas.UserActivity;
import com.arjuna.mw.wsas.UserActivityFactory;

import com.arjuna.mw.wsas.activity.ActivityHierarchy;

import org.w3c.dom.*;

import javax.xml.parsers.*;

import com.arjuna.mw.wsas.exceptions.SystemException;

/**
 * On demand this class creates the SOAP context information necessary to
 * propagate the hierarchy of coordinators associated with the current
 * thread.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: ArjunaContextImple.java,v 1.5 2003/03/04 12:55:57 nmcl Exp $
 */

public class ArjunaContextImple implements SOAPContext
{

    public ArjunaContextImple ()
    {
	_context = null;
    }
    
    public ArjunaContextImple (ACCoordinator currentCoordinator)
    {
	_context = null;

	initialise(currentCoordinator);
    }

    public void initialise (Object param) throws java.security.InvalidParameterException
    {
	try
	{
	    ACCoordinator currentCoordinator = (ACCoordinator) param;
	    DocumentBuilder builder = com.arjuna.mw.wscf.utils.DomUtil.getDocumentBuilder();
	    org.w3c.dom.Document doc = builder.newDocument();

	    _context = doc.createElement("wscoor:"+_contextName);
	
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
		/*
		 * Do the manditory stuff first.
		 */

		ActionHierarchy txHier = currentCoordinator.getHierarchy();
		org.w3c.dom.Element identifier = doc.createElement("wsu:"+_identifier);
		identifier.appendChild(doc.createTextNode(txHier.getDeepestActionUid().stringForm()));
	    
		_context.appendChild(identifier);

		org.w3c.dom.Element expires = doc.createElement("wsu:"+_expires);
		expires.appendChild(doc.createTextNode(""+hier.activity(hier.size()-1).getTimeout()));

		_context.appendChild(expires);

		org.w3c.dom.Element coordinationType = doc.createElement("wscoor:CoordinationType");
		coordinationType.appendChild(doc.createTextNode(_coordinationTypeURI));

		_context.appendChild(coordinationType);
		
		/*
		 * Now let's do the optional stuff.
		 */

		if (txHier.depth() -1 > 0)
		{
		    org.w3c.dom.Element extensionRoot = doc.createElement("arjuna:"+_contextName);

		    for (int i = 0; i < txHier.depth() -1; i++)
		    {
			identifier = doc.createElement("arjuna:"+_identifier);
			identifier.appendChild(doc.createTextNode(txHier.getActionUid(i).stringForm()));

			extensionRoot.appendChild(identifier);
		    
			expires = doc.createElement("arjuna:"+_expires);
			expires.appendChild(doc.createTextNode(""+hier.activity(i).getTimeout()));

			extensionRoot.appendChild(expires);
		    }

		    _context.appendChild(extensionRoot);
		}
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
	return ArjunaContextImple.class.getName();
    }

    public String toString ()
    {
	return DomUtil.nodeAsString(_context);
    }

    private org.w3c.dom.Element _context;

    private static final String _wscoorNamespace = "http://schemas.xmlsoap.org/ws/2002/08/wscoor";
    private static final String _wsuNamespace = "http://schemas.xmlsoap.org/ws/2002/07/utility";
    private static final String _arjunaNamespace = "http://arjuna.com/schemas/wsc/2003/01/extension";
    
    private static final String _contextName = "CoordinationContext";
    private static final String _identifier = "Identifier";
    private static final String _expires = "Expires";
    private static final String _coordinationType = "CoordinationType";

    public static final String _coordinationTypeURI = "urn:arjuna:tx-two-phase-commit";

}
