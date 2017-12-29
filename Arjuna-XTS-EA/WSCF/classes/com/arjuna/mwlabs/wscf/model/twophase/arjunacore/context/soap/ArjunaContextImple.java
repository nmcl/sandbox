/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ArjunaContextImple.java,v 1.4 2003/03/04 12:55:57 nmcl Exp $
 */

package com.arjuna.mwlabs.wscf.model.twophase.arjunacore.context.soap;

import com.arjuna.mwlabs.wscf.model.twophase.arjunacore.ACCoordinator;

import com.hp.mw.ts.arjuna.coordinator.TwoPhaseCoordinator;
import com.hp.mw.ts.arjuna.coordinator.ActionHierarchy;

import com.arjuna.mw.wsas.context.Context;
import com.arjuna.mw.wsas.context.ContextManager;
import com.arjuna.mw.wsas.context.soap.SOAPContext;

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
 * @version $Id: ArjunaContextImple.java,v 1.4 2003/03/04 12:55:57 nmcl Exp $
 */

public class ArjunaContextImple implements SOAPContext
{

    /**
     * Incomplete. Need things like the Coordinator URI!
     */

    public ArjunaContextImple (ACCoordinator currentCoordinator)
    {
	DocumentBuilder builder = DomUtil.getDocumentBuilder();
	org.w3c.dom.Document doc = builder.newDocument();
	
	_context = doc.createElement(_contextName);
	_context.appendChild(doc.createComment("WARNING: example Arjuna coordinator hierarchy only!"));
	
	if (currentCoordinator != null)
	{
	    ActionHierarchy hier = currentCoordinator.getHierarchy();
	
	    for (int i = 0; i < hier.depth(); i++)
	    {
		org.w3c.dom.Element elem = doc.createElement(_coordName);
		
		elem.appendChild(doc.createTextNode("http://arjuna.com?"+hier.getActionUid(i).stringForm()));
		
		_context.appendChild(elem);
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
	return ArjunaContextImple.class.getName();
    }

    public String toString ()
    {
	return DomUtil.nodeAsString(_context);
    }

    private org.w3c.dom.Element _context;
    
    private static final String _contextName = "twophase-arjunacore-context";
    private static final String _coordName = "coordinator-name";

}
