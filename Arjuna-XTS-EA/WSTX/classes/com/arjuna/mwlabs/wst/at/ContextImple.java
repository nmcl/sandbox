/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ContextImple.java,v 1.9 2003/04/04 15:00:55 nmcl Exp $
 */

package com.arjuna.mwlabs.wst.at;

import com.arjuna.wsc.messaging.PortReference;

import com.arjuna.mw.wscf.utils.DomUtil;

import com.arjuna.mwlabs.wsc.common.CoordinationContext;
import com.arjuna.mwlabs.wsc.common.Schema;

import com.arjuna.mw.wsas.context.Context;
import com.arjuna.mw.wsas.context.DeploymentContext;
import com.arjuna.mw.wsas.context.DeploymentContextFactory;
import com.arjuna.mw.wsas.context.soap.SOAPContext;

import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.DocumentBuilder;

import org.w3c.dom.Element;

import com.arjuna.wsc.messaging.AttributedDateTime;

public class ContextImple implements com.arjuna.mw.wsc.context.Context
{

    public ContextImple (com.arjuna.wsc.messaging.CoordinationContext ctx)
    {
        _wscfContext = null;
        _coordContext = ctx;
    }

    public ContextImple (com.arjuna.mw.wsas.context.soap.SOAPContext ctx, PortReference regAddress)
    {
        _wscfContext = ctx;

        CoordinationContext xmlCtx = new CoordinationContext(_wscfContext.context());

	_coordContext = new com.arjuna.wsc.messaging.CoordinationContext();

	_coordContext.identifier = xmlCtx.identifier();
	_coordContext.coordinationType = xmlCtx.coordinationType();
	_coordContext.registrationService = regAddress;

	if (!xmlCtx.expires().equals("0"))
	{
	    AttributedDateTime time = new AttributedDateTime();

	    time.dataTime = xmlCtx.expires();
	    time.valueType = "";
	    
	    _coordContext.expires = time;
	}
    }

    public boolean equals (Object obj)
    {
        if (obj instanceof ContextImple)
        {
            // TODO AttributedDateTime??

            // only compare ids

	    ContextImple ci = (ContextImple) obj;
	    
            return ci.getCoordinationContext().identifier.equals(_coordContext.identifier);
        }
        else
            return false;
    }

    public com.arjuna.wsc.messaging.CoordinationContext getCoordinationContext ()
    {
	return _coordContext;
    }
    
    public void setCoordinationContext (com.arjuna.wsc.messaging.CoordinationContext cc)
    {
	_coordContext = cc;
    }

    public String toString ()
    {
	//	return DomUtil.nodeAsString(toXML());
	
	return "TransactionIdentifier: "+_coordContext.identifier;
    }

    private com.arjuna.mw.wsas.context.soap.SOAPContext  _wscfContext;
    private com.arjuna.wsc.messaging.CoordinationContext _coordContext;

}
