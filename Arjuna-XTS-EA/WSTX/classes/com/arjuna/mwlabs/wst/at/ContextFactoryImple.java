/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ContextFactoryImple.java,v 1.12 2003/04/04 15:00:55 nmcl Exp $
 */

package com.arjuna.mwlabs.wst.at;

import com.arjuna.mw.wst.common.Protocols;

import com.arjuna.mwlabs.wsc.*;

import com.arjuna.wsc.ContextFactory;
import com.arjuna.wsc.InvalidCreateParametersException;
import com.arjuna.wsc.messaging.PortReference;
import com.arjuna.wsc.messaging.AttributedDateTime;
import com.arjuna.wsc.messaging.CoordinationContext;

import com.arjuna.mw.wsas.context.Context;
import com.arjuna.mw.wsas.context.DeploymentContext;
import com.arjuna.mw.wsas.context.DeploymentContextFactory;
import com.arjuna.mw.wsas.context.soap.SOAPContext;

import com.arjuna.mwlabs.wst.at.participants.CleanupSynchronization;

import com.arjuna.mw.wscf.model.twophase.CoordinatorManagerFactory;
import com.arjuna.mw.wscf.model.twophase.api.CoordinatorManager;

import javax.naming.*;

public class ContextFactoryImple implements ContextFactory
{

    public ContextFactoryImple ()
    {
	try
	{
	    _coordManager = CoordinatorManagerFactory.coordinatorManager();
	}
	catch (Exception ex)
	{
	    ex.printStackTrace();
	}

	_address = null;
	_theRegistrar = new RegistrarImple();
    }

    public ContextFactoryImple (RegistrarMapperImple mapper)
    {
	try
	{
	    _coordManager = CoordinatorManagerFactory.coordinatorManager();
	}
	catch (Exception ex)
	{
	    ex.printStackTrace();
	}

	_address = null;
	_theRegistrar = new RegistrarImple(mapper);
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
	_address = new PortReference(serviceAddress);
    }

    // TODO interposition

    /*
     * If there is a context passed through to create then this newly created
     * coordinator should be interposed.
     */

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
		// make sure no transaction is currently associated

		_coordManager.suspend();
		
		int timeout = 0;
		
		try
		{
		    timeout = Integer.parseInt(expires.dataTime);
		}
		catch (NullPointerException ex)
		{
		}
		catch (Exception ex)
		{
		    throw new InvalidCreateParametersException("Invalid timeout: "+expires.dataTime);
		}
		
		_coordManager.begin(timeout);

		DeploymentContext manager = DeploymentContextFactory.deploymentContext();
		com.arjuna.mw.wsas.context.Context theContext = manager.context();

		com.arjuna.mw.wsc.context.Context ctx = new ContextImple((SOAPContext) theContext, _theRegistrar.coordinatorAddress());

		/*
		 * Now add the registrar for this specific coordinator to
		 * the mapper.
		 */

		_coordManager.enlistSynchronization(new CleanupSynchronization(_coordManager.identifier().toString(), _theRegistrar));

		_theRegistrar.associate();

		return ctx.getCoordinationContext();
	    }
	    catch (com.arjuna.mw.wsas.exceptions.NoActivityException ex)
	    {
		// TODO handle properly

		ex.printStackTrace();
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
	// we don't use this as one implementation is registered per type
    }

    public final RegistrarImple registrar ()
    {
	return _theRegistrar;
    }

    private CoordinatorManager _coordManager;
    private PortReference      _address;
    private RegistrarImple     _theRegistrar;

}
