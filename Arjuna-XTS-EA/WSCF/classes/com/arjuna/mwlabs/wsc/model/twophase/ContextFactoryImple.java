/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ContextFactoryImple.java,v 1.9 2003/03/24 10:45:51 nmcl Exp $
 */

package com.arjuna.mwlabs.wsc.model.twophase;

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

import com.arjuna.mwlabs.wsc.model.twophase.participants.CleanupSynchronization;

import com.arjuna.mw.wscf.model.twophase.CoordinatorManagerFactory;
import com.arjuna.mw.wscf.model.twophase.api.CoordinatorManager;

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
	if (coordinationTypeURI.equals(_coordinationTypeURI))
	{
	    // TODO remove the suspend

	    try
	    {
		_coordManager.suspend();
	    
		_coordManager.begin();

		DeploymentContext manager = DeploymentContextFactory.deploymentContext();
		com.arjuna.mw.wsas.context.Context theContext = manager.context();
	    
		com.arjuna.mw.wsc.context.Context ctx = new ContextImple((SOAPContext) theContext, _theRegistrar.address());

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
	    System.err.println("Invalid type URI: < "+_coordinationTypeURI+", "+coordinationTypeURI+" >");

	    throw new InvalidCreateParametersException("Invalid type URI: < "+_coordinationTypeURI+", "+coordinationTypeURI+" >");
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
    
    public static final String _coordinationTypeURI = "urn:arjuna:tx-two-phase-commit";
    
}
