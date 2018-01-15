/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: LocalRegistrarImple.java,v 1.4 2003/02/19 17:29:20 nmcl Exp $
 */

package com.arjuna.mwlabs.wst.at.local;

import com.arjuna.mw.wscf.model.twophase.api.CoordinatorManager;

import com.arjuna.mwlabs.wst.at.participants.*;

import com.arjuna.wst.*;

import com.arjuna.mw.wst.common.Protocols;

import com.arjuna.mw.wsas.activity.*;

import com.arjuna.wsc.Registrar;
import com.arjuna.wsc.AlreadyRegisteredException;
import com.arjuna.wsc.InvalidProtocolException;
import com.arjuna.wsc.InvalidStateException;
import com.arjuna.wsc.NoActivityException;

import com.arjuna.mw.wscf.model.twophase.CoordinatorManagerFactory;
import com.arjuna.mw.wscf.model.twophase.api.CoordinatorManager;

/**
 * This class simulates the use of the real RegistrarImple, which we can't
 * use in a local environment simply because we do not have URIs to register:
 * we have real participants!
 */

public class LocalRegistrarImple
{

    public LocalRegistrarImple ()
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
     * Registers the interest of participant in a particular protocol.
     *
     * @param participantProtocolServiceAddress the address of the participant protocol service
     * @param protocolIdentifier the protocol identifier
     *
     * @throws AlreadyRegisteredException if the participant is already registered for this coordination protocol under
     *         this activity identifier
     * @throws InvalidProtocolException if the coordination protocol is not supported
     * @throws InvalidStateException if the state of the coordinator no longer allows registration for this
     *         coordination protocol
     * @throws NoActivityException if the activity does not exist
     */

    public void register (Object participant, String protocolIdentifier) throws AlreadyRegisteredException, InvalidProtocolException, InvalidStateException, NoActivityException
    {
	// TODO check for AlreadyRegisteredException
	
	if (protocolIdentifier.equals(Protocols.TwoPhaseCommit))
	{
	    // enlist participant that wraps the requester URI.

	    try
	    {
		_coordManager.enlistParticipant(new TwoPhaseCommitParticipant((TwoPCParticipant) participant));
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
		    _coordManager.enlistSynchronization(new PhaseZeroSynchronization((PhaseZeroParticipant) participant));
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
			_coordManager.enlistSynchronization(new OutcomeNotificationSynchronization((OutcomeNotificationParticipant) participant));
		    }
		    catch (Exception ex)
		    {
			throw new InvalidStateException();
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

    private CoordinatorManager _coordManager;

}
