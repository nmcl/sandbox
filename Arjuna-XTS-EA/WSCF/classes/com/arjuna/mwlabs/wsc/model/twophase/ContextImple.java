/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ContextImple.java,v 1.8 2003/02/26 12:05:44 nmcl Exp $
 */

package com.arjuna.mwlabs.wsc.model.twophase;

import com.arjuna.wsc.messaging.PortReference;
import com.arjuna.wsc.messaging.AttributedDateTime;

import com.arjuna.mwlabs.wsc.common.CoordinationContext;

import com.arjuna.mw.wsas.context.Context;
import com.arjuna.mw.wsas.context.DeploymentContext;
import com.arjuna.mw.wsas.context.DeploymentContextFactory;
import com.arjuna.mw.wsas.context.soap.SOAPContext;

// TODO refactor into context package

public class ContextImple implements com.arjuna.mw.wsc.context.Context
{

    public ContextImple (com.arjuna.mw.wsas.context.soap.SOAPContext ctx, PortReference regAddress)
    {
	_wscfContext = ctx;
	
	CoordinationContext xmlCtx = new CoordinationContext(_wscfContext.context());

	_coordContext = new com.arjuna.wsc.messaging.CoordinationContext();

	_coordContext.identifier = xmlCtx.identifier();
	_coordContext.coordinationType = xmlCtx.coordinationType();
	_coordContext.registrationService = regAddress;

	AttributedDateTime time = new AttributedDateTime();
	time.dataTime = xmlCtx.expires();

	_coordContext.expires = time;
    }

    public com.arjuna.wsc.messaging.CoordinationContext getCoordinationContext ()
    {
	return _coordContext;
    }
    
    public void setCoordinationContext (com.arjuna.wsc.messaging.CoordinationContext cc)
    {
	_coordContext = cc;
    }

    public boolean equals (Object obj)
    {
	if (obj instanceof ContextImple)
	{
	    ContextImple ci = (ContextImple) obj;
	    
            return ci.getCoordinationContext().identifier.equals(_coordContext.identifier);
	}
	else
	    return false;
    }
	    
    private com.arjuna.mw.wsas.context.soap.SOAPContext  _wscfContext;
    private com.arjuna.wsc.messaging.CoordinationContext _coordContext;

}
