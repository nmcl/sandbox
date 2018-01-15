/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: LocalContextFactoryImple.java,v 1.3 2003/02/26 12:06:29 nmcl Exp $
 */

package com.arjuna.mwlabs.wst.at.local;

import com.arjuna.mwlabs.wst.at.*;

import com.arjuna.mw.wst.common.Protocols;

import com.arjuna.wsc.ContextFactory;
import com.arjuna.wsc.InvalidCreateParametersException;
import com.arjuna.wsc.messaging.AttributedDateTime;
import com.arjuna.wsc.messaging.CoordinationContext;

import com.arjuna.mw.wsas.context.Context;
import com.arjuna.mw.wsas.context.DeploymentContext;
import com.arjuna.mw.wsas.context.DeploymentContextFactory;
import com.arjuna.mw.wsas.context.soap.SOAPContext;

import com.arjuna.mwlabs.wsc.*;

import com.arjuna.mw.wscf.model.twophase.CoordinatorManagerFactory;
import com.arjuna.mw.wscf.model.twophase.api.CoordinatorManager;

public class LocalContextFactoryImple implements ContextFactory
{

    public LocalContextFactoryImple ()
    {
	try
	{
	    _coordManager = CoordinatorManagerFactory.coordinatorManager();
	}
	catch (Exception ex)
	{
	    ex.printStackTrace();
	}
    }

    /**
     * Called when a context factory is added to a context factory mapper. This method will be called multiple times
     * if the context factory is added to multiple context factory mappers or to the same context mapper with different
     * protocol identifiers.
     *
     * @param serviceAddress the address of the service
     * @param coordinationTypeURI the coordination type uri
     */

    public void install (String serviceAddress, String coordinationTypeURI)
    {
    }

    /**
     * Creates a coordination context.
     *
     * @param coordinationTypeURI the coordination type uri
     * @param expires the expire date/time for the returned context, can be null
     * @param currentContext the current context, can be null
     *
     * @return the created coordination context
     *
     * @throws InvalidCreateParametersException if a parameter passed is invalid
     *         this activity identifier
     */

    public CoordinationContext create (String coordinationTypeURI, AttributedDateTime expires, CoordinationContext currentContext) throws InvalidCreateParametersException
    {
	if (coordinationTypeURI.equals(Protocols.AtomicTransaction))
	{
	    try
	    {
		_coordManager.suspend();

		_coordManager.begin();

		DeploymentContext manager = DeploymentContextFactory.deploymentContext();
		com.arjuna.mw.wsas.context.Context theContext = manager.context();

		com.arjuna.mw.wsc.context.Context ctx = new ContextImple((SOAPContext) theContext, null);

		return ctx.getCoordinationContext();
	    }
	    catch (com.arjuna.mw.wsas.exceptions.SystemException ex)
	    {
		// TODO handle properly

		ex.printStackTrace();
	    }
	    catch (com.arjuna.mw.wsas.exceptions.WrongStateException ex)
	    {
		// TODO handle properly

		ex.printStackTrace();
	    }
	    catch (Exception ex)
	    {
		// TODO handle properly

		ex.printStackTrace();
	    }
	}
	else
	{
	    System.err.println("Invalid type URI: < "+Protocols.AtomicTransaction+", "+coordinationTypeURI+" >");

	    throw new InvalidCreateParametersException("Invalid type URI: < "+Protocols.AtomicTransaction+", "+coordinationTypeURI+" >");
	}

	return null;
    }

    /**
     * Called when a context factory is removed from a context factory mapper. This method will be called multiple
     * times if the context factory is removed from multiple context factory mappers or from the same context factory
     * mapper with different coordination type uris.
     *
     * @param serviceAddress the address of the service
     * @param coordinationTypeURI the coordination type uri
     */

    public void uninstall (String serviceAddress, String coordinationTypeURI)
    {
    }

    public CoordinatorManager coordinatorManager ()
    {
	return _coordManager;
    }

    private CoordinatorManager _coordManager;
    
}
