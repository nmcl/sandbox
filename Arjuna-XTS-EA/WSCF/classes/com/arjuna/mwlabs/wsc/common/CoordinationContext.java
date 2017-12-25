/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: CoordinationContext.java,v 1.2 2003/02/24 13:21:32 nmcl Exp $
 */

package com.arjuna.mwlabs.wsc.common;

import com.arjuna.mw.wscf.utils.DomUtil;

import com.arjuna.wsc.messaging.PortReference;

import javax.xml.parsers.*;

import org.w3c.dom.*;

/**
 * Given a DOM element, an instance of this class extracts the WS-C
 * coordination context from it and makes it directly available.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: CoordinationContext.java,v 1.2 2003/02/24 13:21:32 nmcl Exp $
 * @since 1.0.
 */

public class CoordinationContext
{

    public CoordinationContext (org.w3c.dom.Element context)
    {
	org.w3c.dom.Node regService = DomUtil.getNode(context, Schema.REGISTRATION_SERVICE);
	org.w3c.dom.Text name = null;

	_registrationService = null;

	// TODO use PortReferenceHelper

	/*
	try
	{
	    name = DomUtil.getTextNode(regService, Schema.ADDRESS);
	    
	    if (name != null)
		_registrationService = name.getNodeValue();
	}
	catch (Exception ex)
	{
	}
	*/

	_identifier = null;
	
	try
	{
	    name = DomUtil.getTextNode(context, Schema.IDENTIFIER);
	    
	    if (name != null)
		_identifier = name.getNodeValue();
	}
	catch (Exception ex)
	{
	}
	
	_expires = null;
	
	try
	{
	    name = DomUtil.getTextNode(context, Schema.EXPIRES);
	    
	    if (name != null)
		_expires = name.getNodeValue();
	}
	catch (Exception ex)
	{
	}
	
	_coordinationType = null;
	
	try
	{
	    name = DomUtil.getTextNode(context, Schema.COORDINATION_TYPE);
	    
	    if (name != null)
		_coordinationType = name.getNodeValue();
	}
	catch (Exception ex)
	{
	}
	
	_other = null;
    }

    public final PortReference registrationService ()
    {
	return _registrationService;
    }
    
    public final String expires ()
    {
	return _expires;
    }
    
    public final String identifier ()
    {
	return _identifier;
    }
    
    public final String coordinationType ()
    {
	return _coordinationType;
    }
    
    public final String other ()
    {
	return _other;
    }

    public String toString ()
    {
	return "< "+_registrationService+", "+_identifier+", "+_expires+", "+_coordinationType+", "+_other+" >";
    }

    private PortReference _registrationService;
    private String        _identifier;
    private String        _expires;
    private String        _coordinationType;
    private String        _other;
    
}

