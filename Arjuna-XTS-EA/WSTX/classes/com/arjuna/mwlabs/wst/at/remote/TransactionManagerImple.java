/*
 * Copyright (C) 2003,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: TransactionManagerImple.java,v 1.11 2003/04/11 11:20:59 nmcl Exp $
 */

package com.arjuna.mwlabs.wst.at.remote;

import com.hp.mw.ts.arjuna.common.Uid;

import com.arjuna.mwlabs.wst.at.context.TxContextImple;

import com.arjuna.wsc.messaging.*;
import com.arjuna.wsc.*;

import com.arjuna.wst.*;
import com.arjuna.wst.impl.messaging.XMLUtil;
import com.arjuna.wst.dispatcher.*;

import com.arjuna.mw.wst.TxContext;
import com.arjuna.mw.wst.common.*;

import javax.naming.InitialContext;

/**
 * This is the interface that the core exposes in order to allow different
 * types of participants to be enrolled. The messaging layer continues to
 * work in terms of the registrar, but internally we map to one of these
 * methods.
 */

public class TransactionManagerImple implements com.arjuna.mw.wst.TransactionManager
{

    public TransactionManagerImple ()
    {
	try
	{
	    InitialContext initialContext = new InitialContext();

	    String registrationRequesterJNDIName = System.getProperty(Environment.REGISTRATION_REQUESTER, "registrationrequester");

	    _registrationRequester = (RegistrationRequester) initialContext.lookup(registrationRequesterJNDIName);
	    _registrationRequesterServiceURI = new PortReference(_registrationRequester.getServiceURI());

	    initialise();
	}
	catch (Exception ex)
	{
	    // TODO

	    ex.printStackTrace();
	}
    }
	
    public void enlistForPhaseZero (PhaseZeroParticipant pzp, String id) throws WrongStateException, UnknownTransactionException, AlreadyRegisteredException, SystemException
    {
	try
	{
	    PortReference endPoint = new PortReference(_phaseZeroParticipant.getAddress());
	    
	    registerParticipant(endPoint, id, Protocols.PhaseZero);

	    _phaseZeroDispatcher.activateObject(pzp, id);
	}
	catch (com.arjuna.wsc.InvalidProtocolException ex)
	{
	    throw new SystemException(ex.toString());
	}
	catch (com.arjuna.wsc.InvalidStateException ex)
	{
	    throw new WrongStateException();
	}
	catch (com.arjuna.wsc.NoActivityException ex)
	{
	    throw new UnknownTransactionException();
	}
	finally
	{
	}
	
    }

    public void enlistForTwoPhase (TwoPCParticipant tpp, String id) throws WrongStateException, UnknownTransactionException, AlreadyRegisteredException, SystemException
    {
	try
	{
	    PortReference endPoint = new PortReference(_twoPCParticipant.getAddress());

	    registerParticipant(endPoint, id, Protocols.TwoPhaseCommit);

	    _twoPCDispatcher.activateObject(tpp, id);
	}
	catch (com.arjuna.wsc.InvalidProtocolException ex)
	{
	    throw new SystemException(ex.toString());
	}
	catch (com.arjuna.wsc.InvalidStateException ex)
	{
	    throw new WrongStateException();
	}
	catch (com.arjuna.wsc.NoActivityException ex)
	{
	    ex.printStackTrace();
	    
	    throw new UnknownTransactionException();
	}
    }

    /*
     * TODO
     *
     * Have participant interfaces in WSTX have url method and services use those.
     * The ones in WS-T are message oriented and we translate to/from.
     */

    public void enlistForOutcomeNotification (OutcomeNotificationParticipant onp, String id) throws WrongStateException, UnknownTransactionException, AlreadyRegisteredException, SystemException
    {
	try
	{
	    PortReference endPoint = new PortReference(_outcomeNotificationParticipant.getAddress());

	    registerParticipant(endPoint, id, Protocols.OutcomeNotification);

	    _outcomeNotificationDispatcher.activateObject(onp, id);
	}
	catch (com.arjuna.wsc.InvalidProtocolException ex)
	{
	    throw new SystemException(ex.toString());
	}
	catch (com.arjuna.wsc.InvalidStateException ex)
	{
	    throw new WrongStateException();
	}
	catch (com.arjuna.wsc.NoActivityException ex)
	{
	    throw new UnknownTransactionException();
	}
    }
    
    public int replay () throws SystemException
    {
	throw new SystemException("Not implemented!");
    }

    public static javax.naming.Reference getReference ()
    {
	return new javax.naming.Reference(TransactionManagerImple.class.getName());
    }

    public TxContext suspend () throws SystemException
    {
	return _ctxManager.suspend();
    }

    // resume overwrites. Should we check first a la JTA?

    public void resume (TxContext tx) throws UnknownTransactionException, SystemException
    {
	_ctxManager.resume(tx);
    }

    public TxContext currentTransaction () throws SystemException
    {
	return _ctxManager.currentTransaction();
    }

    protected PortReference enlistForCompletion (String url) throws WrongStateException, UnknownTransactionException, AlreadyRegisteredException, SystemException
    {
	try
	{
	    TxContextImple currentTx = (TxContextImple) _ctxManager.currentTransaction();
	    
	    if (currentTx == null)
		throw new com.arjuna.wsc.NoActivityException();

	    return registerParticipant(new PortReference(url), currentTx.identifier(), Protocols.Completion);
	}
	catch (com.arjuna.wsc.InvalidProtocolException ex)
	{
	    ex.printStackTrace();
	    
	    throw new SystemException(ex.toString());
	}
	catch (com.arjuna.wsc.InvalidStateException ex)
	{
	    throw new WrongStateException();
	}
	catch (com.arjuna.wsc.NoActivityException ex)
	{
	    throw new UnknownTransactionException();
	}
    }

    protected PortReference enlistForCompletionWithAck (String url) throws WrongStateException, UnknownTransactionException, AlreadyRegisteredException, SystemException
    {
	try
	{
	    TxContextImple currentTx = (TxContextImple) _ctxManager.currentTransaction();
	    
	    if (currentTx == null)
		throw new com.arjuna.wsc.NoActivityException();

	    return registerParticipant(new PortReference(url), currentTx.identifier(), Protocols.CompletionWithAck);
	}
	catch (com.arjuna.wsc.InvalidProtocolException ex)
	{
	    ex.printStackTrace();
	    
	    throw new SystemException(ex.toString());
	}
	catch (com.arjuna.wsc.InvalidStateException ex)
	{
	    throw new WrongStateException();
	}
	catch (com.arjuna.wsc.NoActivityException ex)
	{
	    throw new UnknownTransactionException();
	}
    }
	
    private final PortReference registerParticipant (PortReference endPoint, String id, String protocol) throws com.arjuna.wsc.InvalidProtocolException, com.arjuna.wsc.InvalidStateException, com.arjuna.wsc.NoActivityException, SystemException
    {
	try
	{
	    TxContextImple currentTx = (TxContextImple) _ctxManager.currentTransaction();
	    
	    if (currentTx == null)
		throw new com.arjuna.wsc.NoActivityException();

	    RegisterMessage sentRegisterMessage = new RegisterMessage();
	    RegisterResponseMessage receivedRegisterResponseMessage = null;

	    sentRegisterMessage.registrationService = new PortReference(currentTx.context().getCoordinationContext().registrationService.getAddress());
	    sentRegisterMessage.registrationService.addExtension(com.arjuna.wsc.impl.messaging.XMLUtil.INSTANCE_IDENTIFIER_TAG, currentTx.identifier());
	    sentRegisterMessage.requesterReference = _registrationRequesterServiceURI;
	    sentRegisterMessage.protocolIdentifier = protocol;
	    sentRegisterMessage.participantProtocolService = endPoint;
	    sentRegisterMessage.participantProtocolService.addExtension(XMLUtil.PARTICIPANT_IDENTIFIER_TAG, id);
	    sentRegisterMessage.dialogIdentifier = new Uid().stringForm();
	    _registrationRequester.sendRegister(sentRegisterMessage);
	    
	    receivedRegisterResponseMessage = _registrationRequester.recvRegisterResponse(sentRegisterMessage.dialogIdentifier);

	    if (!receivedRegisterResponseMessage.requesterReference.getAddress().equals(_registrationRequesterServiceURI.getAddress()))
	    {
		throw new InvalidProtocolException("Received address does not equal our address!");
	    }
	    else
		return receivedRegisterResponseMessage.coordinationProtocolService;
	}
	catch (MarshalException ex)
	{
	    throw new SystemException(ex.toString());
	}
	catch (com.arjuna.wsc.NoActivityException ex)
	{
	    throw ex;
	}
	catch (Exception ex)
	{
	    throw new SystemException(ex.toString());
	}
    }

    private final void initialise () throws Exception
    {
	InitialContext initialContext = new InitialContext();

	String twoPCDispatcherJNDIName = System.getProperty(Environment.TWOPC_DISPATCHER, "twopcdispatcher");
	String twoPCParticipantJNDIName = System.getProperty(Environment.TWOPC_PARTICIPANT, "twopcparticipant");
	String phaseZeroDispatcherJNDIName = System.getProperty(Environment.PHASEZERO_DISPATCHER, "phasezerodispatcher");
	String phaseZeroParticipantJNDIName = System.getProperty(Environment.PHASEZERO_PARTICIPANT, "phasezeroparticipant");
	String outcomeNotificationDispatcherJNDIName = System.getProperty(Environment.OUTCOMENOTIFICATION_DISPATCHER, "outcomenotificationdispatcher");
	String outcomeNotificationParticipantJNDIName = System.getProperty(Environment.OUTCOMENOTIFICATION_PARTICIPANT, "outcomenotificationparticipant");
	
	_twoPCDispatcher = (TwoPCDispatcher) initialContext.lookup(twoPCDispatcherJNDIName);
	_twoPCParticipant = new PortReference(((com.arjuna.wst.messaging.TwoPCParticipant) initialContext.lookup(twoPCParticipantJNDIName)).getServiceURI());
	_phaseZeroDispatcher = (PhaseZeroDispatcher) initialContext.lookup(phaseZeroDispatcherJNDIName);
	_phaseZeroParticipant = new PortReference(((com.arjuna.wst.messaging.PhaseZeroParticipant) initialContext.lookup(phaseZeroParticipantJNDIName)).getServiceURI());
	_outcomeNotificationDispatcher = (OutcomeNotificationDispatcher) initialContext.lookup(outcomeNotificationDispatcherJNDIName);
	_outcomeNotificationParticipant = new PortReference(((com.arjuna.wst.messaging.OutcomeNotificationParticipant) initialContext.lookup(outcomeNotificationParticipantJNDIName)).getServiceURI());
    }
    
    private ContextManager _ctxManager = new ContextManager();

    private RegistrationRequester _registrationRequester;
    private PortReference         _registrationRequesterServiceURI;

    private TwoPCDispatcher _twoPCDispatcher = null;
    private PortReference _twoPCParticipant = null;
    private PhaseZeroDispatcher _phaseZeroDispatcher = null;
    private PortReference _phaseZeroParticipant = null;
    private OutcomeNotificationDispatcher _outcomeNotificationDispatcher = null;
    private PortReference _outcomeNotificationParticipant = null;
    
}
