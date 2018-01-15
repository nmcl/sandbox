/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: RegistrarImple.java,v 1.12 2003/04/04 15:00:55 nmcl Exp $
 */

package com.arjuna.mwlabs.wst.at;

import com.arjuna.wsc.RegistrarMapper;

import com.arjuna.mwlabs.wsc.*;

import com.arjuna.mwlabs.wst.at.participants.*;

import com.arjuna.mw.wst.common.Protocols;

import com.arjuna.wst.*;
import com.arjuna.wst.messaging.*;
import com.arjuna.wst.stub.*;
import com.arjuna.wst.dispatcher.*;

import com.arjuna.mw.wsas.activity.*;

import com.arjuna.wsc.Registrar;
import com.arjuna.wsc.AlreadyRegisteredException;
import com.arjuna.wsc.InvalidProtocolException;
import com.arjuna.wsc.InvalidStateException;
import com.arjuna.wsc.NoActivityException;

import com.arjuna.wst.impl.messaging.XMLUtil;

import com.arjuna.wsc.messaging.PortReference;

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
	    
	    RegistrarMapper mapper = null;

	    try
	    {
		mapper = (RegistrarMapper) initialContext.lookup(RegistrarMapperImple.localName());
	    }
	    catch (Exception ex)
	    {
		mapper = null;
	    }
	    
	    if (mapper == null)
		mapper = (RegistrarMapper) initialContext.lookup("registrarmapper");

	    // register with mapper using tx id as protocol identifier.

	    mapper.addRegistrar(Protocols.PhaseZero, this);
	    mapper.addRegistrar(Protocols.TwoPhaseCommit, this);
	    mapper.addRegistrar(Protocols.OutcomeNotification, this);
	    mapper.addRegistrar(Protocols.Completion, this);
	    mapper.addRegistrar(Protocols.CompletionWithAck, this);

	    initialise();
	}
	catch (Exception ex)
	{
	    ex.printStackTrace();
	}
    }
    
    public RegistrarImple (RegistrarMapperImple mapper)
    {
	try
	{
	    _coordManager = CoordinatorManagerFactory.coordinatorManager();

	    // register with mapper using tx id as protocol identifier.

	    mapper.addRegistrar(Protocols.PhaseZero, this);
	    mapper.addRegistrar(Protocols.TwoPhaseCommit, this);
	    mapper.addRegistrar(Protocols.OutcomeNotification, this);
	    mapper.addRegistrar(Protocols.Completion, this);
	    mapper.addRegistrar(Protocols.CompletionWithAck, this);

	    initialise();
	}
	catch (Exception ex)
	{
	    ex.printStackTrace();
	}
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
	_address = new PortReference(serviceAddress);
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

	// TODO check for AlreadyRegisteredException
	
	if (protocolIdentifier.equals(Protocols.TwoPhaseCommit))
	{
	    // enlist participant that wraps the requester URI.

	    try
	    {
		_coordManager.enlistParticipant(new TwoPhaseCommitParticipant(new TwoPCStub(_twoPCCoordinator, participantProtocolService)));

		PortReference twoPCCoordinatorAddress = new PortReference(_twoPCCoordinator.getServiceURI());
		    
		twoPCCoordinatorAddress.addExtension(XMLUtil.TRANSACTION_IDENTIFIER_TAG, additionalInformation);

		_coordManager.suspend();
		
		return twoPCCoordinatorAddress;
	    }
	    catch (Exception ex)
	    {
		throw new InvalidStateException();
	    }
	}
	else
	{
	    if (protocolIdentifier.equals(Protocols.PhaseZero))
	    {
		try
		{
		    _coordManager.enlistSynchronization(new PhaseZeroSynchronization(new PhaseZeroStub(_phaseZeroCoordinator, participantProtocolService)));

		    PortReference phaseZeroCoordinatorAddress = new PortReference(_phaseZeroCoordinator.getServiceURI());
		    
		    phaseZeroCoordinatorAddress.addExtension(XMLUtil.TRANSACTION_IDENTIFIER_TAG, additionalInformation);

		    _coordManager.suspend();
		    
		    return phaseZeroCoordinatorAddress;
		}
		catch (Exception ex)
		{
		    throw new InvalidStateException();
		}
	    }
	    else
	    {
		if (protocolIdentifier.equals(Protocols.OutcomeNotification))
		{
		    try
		    {
			_coordManager.enlistSynchronization(new OutcomeNotificationSynchronization(new OutcomeNotificationStub(_outcomeNotificationCoordinator, participantProtocolService)));
			
			PortReference outcomeNotificationCoordinatorAddress = new PortReference(_outcomeNotificationCoordinator.getServiceURI());
		    
			outcomeNotificationCoordinatorAddress.addExtension(XMLUtil.TRANSACTION_IDENTIFIER_TAG, additionalInformation);
			
			_coordManager.suspend();
		    
			return outcomeNotificationCoordinatorAddress;
		    }
		    catch (Exception ex)
		    {
			throw new InvalidStateException();
		    }
		}
		else
		{
		    if (protocolIdentifier.equals(Protocols.Completion))
		    {
			try
			{
			    _completionDispatcher.activateObject(new CompletionCoordinatorImple(_coordManager, hier, _completionDispatcher), additionalInformation);
			
			    PortReference completionCoordinatorAddress = new PortReference(_completionCoordinator.getServiceURI());
			
			    completionCoordinatorAddress.addExtension(XMLUtil.TRANSACTION_IDENTIFIER_TAG, additionalInformation);

			    _coordManager.suspend();

			    return completionCoordinatorAddress;
			}
			catch (Exception ex)
			{
			    ex.printStackTrace();
			    
			    throw new InvalidStateException(ex.toString());
			}
		    }
		    else
		    {
			if (protocolIdentifier.equals(Protocols.CompletionWithAck))
			{
			    try
			    {
				_completionWithAckDispatcher.activateObject(new CompletionWithAckCoordinatorImple(_coordManager, hier, _completionWithAckDispatcher), additionalInformation);
			
				PortReference completionWithAckCoordinatorAddress = new PortReference(_completionWithAckCoordinator.getServiceURI());
			
				completionWithAckCoordinatorAddress.addExtension(XMLUtil.TRANSACTION_IDENTIFIER_TAG, additionalInformation);

				_coordManager.suspend();

				return completionWithAckCoordinatorAddress;
			    }
			    catch (Exception ex)
			    {
				ex.printStackTrace();
				
				throw new InvalidStateException(ex.toString());
			    }
			}
			else
			{
			    System.err.println("Invalid type URI: < "+Protocols.AtomicTransaction+", "+protocolIdentifier+" >");

			    throw new InvalidProtocolException();
			}
		    }
		}
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
	// TODO colocation won't do suspend

	String txIdentifier = _coordManager.identifier().toString();
	ActivityHierarchy hier = _coordManager.suspend();

	_hierarchies.put(txIdentifier, hier);
    }

    public final void disassociate (String txIdentifier) throws Exception
    {
	_hierarchies.remove(txIdentifier);
    }

    public final PortReference coordinatorAddress ()
    {
	return _address;
    }
    
    private final void initialise () throws Exception
    {
	InitialContext initialContext = new InitialContext();

	String phaseZeroJNDIName = "phasezerocoordinator";  // TODO
	String twoPCJNDIName = "twopccoordinator";  // TODO
	String outcomeNotificationJNDIName = "outcomenotificationcoordinator";  // TODO
	String completionCoordinatorJNDIName = "completioncoordinator";
	String completionWithAckCoordinatorJNDIName = "completionwithackcoordinator";
	
	String completionDispatcherJNDIName = "completiondispatcher";
	String completionWithAckDispatcherJNDIName = "completionwithackdispatcher";
	
	_phaseZeroCoordinator = (PhaseZeroCoordinator) initialContext.lookup(phaseZeroJNDIName);
	_twoPCCoordinator = (TwoPCCoordinator) initialContext.lookup(twoPCJNDIName);
	_outcomeNotificationCoordinator = (OutcomeNotificationCoordinator) initialContext.lookup(outcomeNotificationJNDIName);
	_completionCoordinator = (CompletionCoordinator) initialContext.lookup(completionCoordinatorJNDIName);
	_completionWithAckCoordinator = (CompletionWithAckCoordinator) initialContext.lookup(completionWithAckCoordinatorJNDIName);

	_coordinatorReference = new PortReference(_completionCoordinator.getServiceURI());
	
	_completionDispatcher = (CompletionDispatcher) initialContext.lookup(completionDispatcherJNDIName);
	_completionWithAckDispatcher = (CompletionWithAckDispatcher) initialContext.lookup(completionWithAckDispatcherJNDIName);
    }
    
    private CoordinatorManager _coordManager = null;
    private PortReference      _address = null;
    private HashMap            _hierarchies = new HashMap();
    private PortReference      _coordinatorReference = null;
    
    private PhaseZeroCoordinator           _phaseZeroCoordinator = null;
    private TwoPCCoordinator               _twoPCCoordinator = null;
    private OutcomeNotificationCoordinator _outcomeNotificationCoordinator = null;
    private CompletionCoordinator          _completionCoordinator = null;
    private CompletionWithAckCoordinator   _completionWithAckCoordinator = null;
    
    private CompletionDispatcher        _completionDispatcher = null;
    private CompletionWithAckDispatcher _completionWithAckDispatcher = null;
    
}
