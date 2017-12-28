/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: RegistrarImple.java,v 1.5 2003/02/24 13:21:33 nmcl Exp $
 */

package com.arjuna.mwlabs.wsc.model.twophase;

import com.arjuna.mwlabs.wsc.*;

import com.arjuna.mw.wsas.activity.*;

import com.arjuna.wsc.Registrar;
import com.arjuna.wsc.AlreadyRegisteredException;
import com.arjuna.wsc.InvalidProtocolException;
import com.arjuna.wsc.InvalidStateException;
import com.arjuna.wsc.NoActivityException;

import com.arjuna.wsc.messaging.PortReference;

import com.arjuna.mwlabs.wsc.model.twophase.participants.*;

import com.arjuna.mw.wscf.model.twophase.CoordinatorManagerFactory;
import com.arjuna.mw.wscf.model.twophase.api.CoordinatorManager;

import javax.naming.*;

import java.util.HashMap;

public class RegistrarImple implements Registrar
{

    public RegistrarImple ()
    {
	try
	{
	    _coordManager = CoordinatorManagerFactory.coordinatorManager();

	    InitialContext initialContext = new InitialContext();
	    
	    RegistrarMapperImple mapper = (RegistrarMapperImple) initialContext.lookup(RegistrarMapperImple.localName());

	    mapper.addRegistrar(_2PCProtocolId, this);
	    mapper.addRegistrar(_synchProtocolId, this);
	}
	catch (Exception ex)
	{
	    ex.printStackTrace();
	}

	_address = null;
	_coordinatorAddress = new PortReference(System.getProperty(TWO_PHASE_DEPLOYMENT_URL));
    }

    /**
     * Called when a registrar is added to a register mapper. This method will be called multiple times if the
     * registrar is added to multiple register mappers or to the same register mapper with different protocol
     * identifiers.
     *
     * @param serviceAddress the address of the service
     * @param protocolIdentifier the protocol identifier
     */

    public void install (String serviceAddress, String protocolIdentifier)
    {
	_address = serviceAddress;
    }

    /**
     * Registers the interest of participant in a particular protocol.
     *
     * @param participantProtocolServiceAddress the address of the participant protocol service
     * @param protocolIdentifier the protocol identifier
     *
     * @return the PortReference of the coordinator protocol service
     *
     * @throws AlreadyRegisteredException if the participant is already registered for this coordination protocol under
     *         this activity identifier
     * @throws InvalidProtocolException if the coordination protocol is not supported
     * @throws InvalidStateException if the state of the coordinator no longer allows registration for this
     *         coordination protocol
     * @throws NoActivityException if the activity does not exist
     */

    public PortReference register (PortReference participantProtocolService, String protocolIdentifier, String additionalInformation) throws AlreadyRegisteredException, InvalidProtocolException, InvalidStateException, NoActivityException
    {
	ActivityHierarchy hier = (ActivityHierarchy) _hierarchies.get(additionalInformation);

	if (hier == null)
	    throw new NoActivityException();
	
	if (protocolIdentifier.equals(_2PCProtocolId))
	{
	    // enlist participant that wraps the requester URI.

	    try
	    {
		_coordManager.resume(hier);
	    }
	    catch (com.arjuna.mw.wsas.exceptions.InvalidActivityException ex)
	    {
		throw new NoActivityException();
	    }
	    catch (com.arjuna.mw.wsas.exceptions.SystemException ex)
	    {
		throw new InvalidProtocolException();
	    }

	    try
	    {
		// TODO check for AlreadyRegisteredException

		_coordManager.enlistParticipant(new TwoPhaseParticipant(participantProtocolService));
		
		_coordManager.suspend();

		return _coordinatorAddress;
	    }
	    catch (Exception ex)
	    {
		throw new InvalidStateException();
	    }
	}
	else
	{
	    if (protocolIdentifier.equals(_synchProtocolId))
	    {
		try
		{
		    _coordManager.enlistSynchronization(new TwoPhaseSynchronization(participantProtocolService));

		    _coordManager.suspend();

		    return _coordinatorAddress;
		}
		catch (Exception ex)
		{
		    throw new InvalidStateException();
		}
	    }
	    else
	    {
		System.err.println("Invalid type URI: < "+_coordinationTypeURI+", "+protocolIdentifier+" >");

		throw new InvalidProtocolException();
	    }
	}
    }

    /**
     * Called when a registrar is removed from a register mapper. This method will be called multiple times if the
     * registrar is removed from multiple register mappers or from the same register mapper with different protocol
     * identifiers.
     *
     * @param serviceAddress the address of the service
     * @param protocolIdentifier the protocol identifier
     */

    public void uninstall (String serviceAddress, String protocolIdentifier)
    {
    }

    public final void associate () throws Exception
    {
	String txIdentifier = _coordManager.identifier().toString();
	ActivityHierarchy hier = _coordManager.suspend();

	_hierarchies.put(txIdentifier, hier);
    }
    
    public final void disassociate (String txIdentifier) throws Exception
    {
	_hierarchies.remove(txIdentifier);
    }

    public final PortReference address ()
    {
	return _coordinatorAddress;
    }
    
    private CoordinatorManager _coordManager;
    private String             _address;
    private HashMap            _hierarchies = new HashMap();
    private PortReference      _coordinatorAddress;

    public static final String TWO_PHASE_DEPLOYMENT_URL = "com.arjuna.mwlabs.wsc.model.twophase.deploymentURL";
    
    public static final String _coordinationTypeURI = "urn:arjuna:tx-two-phase-commit";
    public static final String _2PCProtocolId = "urn:arjuna:tx-two-phase-commit#2pc";
    public static final String _synchProtocolId = "urn:arjuna:tx-two-phase-commit#synch";

}
