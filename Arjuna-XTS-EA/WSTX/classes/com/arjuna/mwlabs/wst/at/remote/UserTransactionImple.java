/*
 * Copyright (C) 2003,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: UserTransactionImple.java,v 1.12 2003/04/11 11:20:59 nmcl Exp $
 */

package com.arjuna.mwlabs.wst.at.remote;

import com.hp.mw.ts.arjuna.common.Uid;

import com.arjuna.wsc.messaging.*;
import com.arjuna.wsc.messaging.AttributedDateTime;

import com.arjuna.mwlabs.wsc.*;

import com.arjuna.mwlabs.wst.at.context.*;
import com.arjuna.mwlabs.wst.at.ContextImple;

import com.arjuna.mw.wst.common.*;

import com.arjuna.wst.*;
import com.arjuna.wst.messaging.CompletionParticipant;
import com.arjuna.wst.messaging.CompletionWithAckParticipant;
import com.arjuna.wst.messaging.CompletionCoordinator;
import com.arjuna.wst.messaging.CommittedMessage;
import com.arjuna.wst.messaging.AbortedMessage;
import com.arjuna.wst.stub.*;
import com.arjuna.wst.impl.messaging.XMLUtil;

import com.arjuna.mw.wst.*;

import javax.naming.InitialContext;

import java.util.*;

/**
 * This is the interface that allows transactions to be started and terminated.
 * The messaging layer converts the Commit, Rollback and Notify messages into
 * calls on this.
 */

public class UserTransactionImple implements com.arjuna.mw.wst.UserTransaction
{

    public UserTransactionImple ()
    {
	try
	{
	    InitialContext initialContext = new InitialContext();

	    String activationRequesterJNDIName   = System.getProperty(Environment.ACTIVATION_REQUESTER, "activationrequester");
	    String completionParticipantJNDIName = System.getProperty(Environment.COMPLETION_PARTICIPANT, "completionparticipant");
	    String completionWithAckParticipantJNDIName = System.getProperty(Environment.COMPLETION_WITH_ACK_PARTICIPANT, "completionwithackparticipant");

	    String coordinatorUrl = System.getProperty(Environment.COORDINATOR_URL);

	    /*
	     * If the coordinator URL hasn't been specified via the
	     * configuration file then assume we are using a locally registered
	     * implementation (JNDI).
	     */

	    if (coordinatorUrl == null)
	    {
		String activationCoordinatorJNDIName = System.getProperty(Environment.ACTIVATION_COORDINATOR, "activationcoordinator");
		
		ActivationCoordinator accoord = (ActivationCoordinator) initialContext.lookup(activationCoordinatorJNDIName);
		
		coordinatorUrl = accoord.getServiceURI();
	    }

	    _activationRequester = (ActivationRequester) initialContext.lookup(activationRequesterJNDIName);
	    _activationRequesterServiceURI = new PortReference(_activationRequester.getServiceURI());
	    _activationCoordinatorServiceURI = new PortReference(coordinatorUrl);

	    /*
	     * The participant end-point address for the coordinator to
	     * call back to us with the result.
	     */

	    _completionParticipant = (CompletionParticipant) initialContext.lookup(completionParticipantJNDIName);

	    _completionWithAckParticipant = (CompletionWithAckParticipant) initialContext.lookup(completionWithAckParticipantJNDIName);
	}
	catch (Exception ex)
	{
	    // TODO

	    ex.printStackTrace();
	}
    }
    
    public void begin () throws WrongStateException, SystemException
    {
	begin(0);
    }
    
    public void begin (int timeout) throws WrongStateException, SystemException
    {
	try
	{
	    if (_ctxManager.currentTransaction() != null)
		throw new WrongStateException();

	    com.arjuna.mw.wsc.context.Context ctx = startTransaction(timeout);

	    _ctxManager.resume(new TxContextImple(ctx));

	    enlistCompletionParticipants();
	}
	catch (com.arjuna.wsc.InvalidCreateParametersException ex)
	{
	    tidyup();

	    throw new SystemException(ex.toString());
	}
	catch (com.arjuna.wst.UnknownTransactionException ex)
	{
	    tidyup();
	    
	    throw new SystemException(ex.toString());
	}
	catch (SystemException ex)
	{
	    tidyup();

	    throw ex;
	}
    }

    public void commit () throws TransactionRolledBackException, UnknownTransactionException, SystemException
    {
	commit(false);
    }

    public void commit (boolean withAck) throws TransactionRolledBackException, UnknownTransactionException, SystemException
    {
	try
	{
	    if (!withAck)
		commitWithoutAck();
	    else
		commitWithAck();
	}
	catch (SystemException ex)
	{
	    throw ex;
	}
	finally
	{
	    if (!withAck)
		tidyup();
	}
    }
    
    public void rollback () throws UnknownTransactionException, SystemException
    {
	rollback(false);
    }
    
    public void rollback (boolean withAck) throws UnknownTransactionException, SystemException
    {
	try
	{
	    if (!withAck)
		abortWithoutAck();
	    else
		abortWithAck();
	}
	catch (SystemException ex)
	{
	    throw ex;
	}
	finally
	{
	    if (!withAck)
		tidyup();
	}
    }
    
    /**
     * Pass the id of the transaction to release. The alternative way would
     * be to not disassociate the transaction from the thread when commit
     * is called (with ack), but this means that the user cannot start another
     * transaction until release is called.
     */

    public void release () throws UnknownTransactionException, SystemException
    {
	try
	{
	    _completionWithAckCoordinatorURL.addExtension(XMLUtil.TRANSACTION_IDENTIFIER_TAG, _ctxManager.currentTransaction().toString());

	    CompletionWithAckStub completionWithAckStub = new CompletionWithAckStub(_completionWithAckParticipant, _completionWithAckCoordinatorURL);

	    completionWithAckStub.notified();
	}
	catch (SystemException ex)
	{
	    throw ex;
	}
	catch (Exception ex)
	{
	    throw new SystemException(ex.toString());
	}
	finally
	{
	    tidyup();
	}
    }

    public String transactionIdentifier ()
    {
	try
	{
	    return _ctxManager.currentTransaction().toString();
	}
	catch (com.arjuna.wst.SystemException ex)
	{
	    return "Unknown";
	}
	catch (NullPointerException ex)
	{
	    return "Unknown";
	}
    }

    public String toString ()
    {
	return transactionIdentifier();
    }

    public static javax.naming.Reference getReference ()
    {
	return new javax.naming.Reference(UserTransactionImple.class.getName());
    }

    /*
     * Not sure if this is right as it doesn't map to registering a participant
     * with the coordinator.
     */

    private final void enlistCompletionParticipants () throws WrongStateException, UnknownTransactionException, SystemException
    {
	try
	{
	    TransactionManagerImple tm = (TransactionManagerImple) TransactionManagerFactory.transactionManager();
	
	    _completionCoordinatorURL = tm.enlistForCompletion(_completionParticipant.getServiceURI());

	    _completionWithAckCoordinatorURL = tm.enlistForCompletionWithAck(_completionWithAckParticipant.getServiceURI());
	}
	catch (com.arjuna.wsc.AlreadyRegisteredException ex)
	{
	    throw new SystemException(ex.toString());
	}
    }
    
    private final com.arjuna.mw.wsc.context.Context startTransaction (int timeout) throws com.arjuna.wsc.InvalidCreateParametersException, SystemException
    {
	try
	{
	    CreateCoordinationContextMessage sentCreateCoordinationContextMessage = new CreateCoordinationContextMessage();
	    CreateCoordinationContextResponseMessage receivedCreateCoordinationContextResponseMessage = null;

	    AttributedDateTime expires = new AttributedDateTime();
	    
	    expires.dataTime = Integer.toString(timeout);
	    expires.valueType = "";
	    
	    sentCreateCoordinationContextMessage.activationService = _activationCoordinatorServiceURI;
	    sentCreateCoordinationContextMessage.requesterReference = _activationRequesterServiceURI;
	    sentCreateCoordinationContextMessage.coordinationType = Protocols.AtomicTransaction;
	    sentCreateCoordinationContextMessage.dialogIdentifier = new Uid().stringForm();
	    sentCreateCoordinationContextMessage.expires = expires;

	    _activationRequester.sendCreateCoordinationContext(sentCreateCoordinationContextMessage);

	    receivedCreateCoordinationContextResponseMessage = _activationRequester.recvCreateCoordinationContextResponse(sentCreateCoordinationContextMessage.dialogIdentifier);

	    if (!receivedCreateCoordinationContextResponseMessage.requesterReference.getAddress().equals(_activationRequesterServiceURI.getAddress()))
	    {
		throw new SystemException("Invalid address");
	    }
	    else
	    {
		CoordinationContext cctx = receivedCreateCoordinationContextResponseMessage.coordinationContext;
		
		if (cctx == null)
		    throw new SystemException("Received context is null!");
		else
		{
		    return new ContextImple(cctx);
		}
	    }
	}
	catch (SystemException ex)
	{
	    throw ex;
	}
	catch (Exception ex)
	{
	    ex.printStackTrace();
	    
	    // TODO

	    throw new SystemException(ex.toString());
	}
    }
    
    private final void commitWithoutAck () throws TransactionRolledBackException, UnknownTransactionException, SystemException
    {
	try
	{
	    _completionCoordinatorURL.addExtension(XMLUtil.TRANSACTION_IDENTIFIER_TAG, _ctxManager.currentTransaction().toString());

	    CompletionStub completionStub = new CompletionStub(_completionParticipant, _completionCoordinatorURL);
	    
	    completionStub.commit();
	}
	catch (SystemException ex)
	{
	    throw ex;
	}
	catch (TransactionRolledBackException ex)
	{
	    throw ex;
	}
	catch (UnknownTransactionException ex)
	{
	    throw ex;
	}
	catch (Exception ex)
	{
	    throw new SystemException(ex.toString());
	}
    }

    private final void commitWithAck () throws TransactionRolledBackException, UnknownTransactionException, SystemException
    {
	try
	{
	    _completionWithAckCoordinatorURL.addExtension(XMLUtil.TRANSACTION_IDENTIFIER_TAG, _ctxManager.currentTransaction().toString());

	    CompletionWithAckStub completionWithAckStub = new CompletionWithAckStub(_completionWithAckParticipant, _completionWithAckCoordinatorURL);
	    
	    completionWithAckStub.commit();
	}
	catch (SystemException ex)
	{
	    throw ex;
	}
	catch (TransactionRolledBackException ex)
	{
	    throw ex;
	}
	catch (UnknownTransactionException ex)
	{
	    throw ex;
	}
	catch (Exception ex)
	{
	    throw new SystemException(ex.toString());
	}
    }

    private final void abortWithoutAck () throws UnknownTransactionException, SystemException
    {
	try
	{
	    _completionCoordinatorURL.addExtension(XMLUtil.TRANSACTION_IDENTIFIER_TAG, _ctxManager.currentTransaction().toString());

	    CompletionStub completionStub = new CompletionStub(_completionParticipant, _completionCoordinatorURL);

	    completionStub.rollback();
	}
	catch (SystemException ex)
	{
	    throw ex;
	}
	catch (UnknownTransactionException ex)
	{
	    throw ex;
	}
	catch (Exception ex)
	{
	    throw new SystemException(ex.toString());
	}
    }

    private final void abortWithAck () throws UnknownTransactionException, SystemException
    {
	try
	{
	    _completionWithAckCoordinatorURL.addExtension(XMLUtil.TRANSACTION_IDENTIFIER_TAG, _ctxManager.currentTransaction().toString());

	    CompletionWithAckStub completionWithAckStub = new CompletionWithAckStub(_completionWithAckParticipant, _completionWithAckCoordinatorURL);

	    completionWithAckStub.rollback();
	}
	catch (SystemException ex)
	{
	    throw ex;
	}
	catch (UnknownTransactionException ex)
	{
	    throw ex;
	}
	catch (Exception ex)
	{
	    throw new SystemException(ex.toString());
	}
    }

    private final void tidyup ()
    {
	try
	{
	    _ctxManager.suspend();
	}
	catch (Exception ex)
	{
	    ex.printStackTrace();
	}
    }
    
    private ContextManager _ctxManager = new ContextManager();
    
    private ActivationRequester          _activationRequester;
    private PortReference                _activationRequesterServiceURI;
    private PortReference                _activationCoordinatorServiceURI;
    private CompletionParticipant        _completionParticipant = null;
    private PortReference                _completionCoordinatorURL = null;
    private CompletionWithAckParticipant _completionWithAckParticipant = null;
    private PortReference                _completionWithAckCoordinatorURL = null;
    
}
