/*
 * Copyright (C) 2003,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: UserTransactionImple.java,v 1.14 2003/04/04 15:00:56 nmcl Exp $
 */

package com.arjuna.mwlabs.wst.at.local;

import com.arjuna.mwlabs.wsc.*;

import com.arjuna.mwlabs.wst.at.context.*;

import com.arjuna.mw.wst.common.Protocols;

import com.arjuna.wst.*;

import com.arjuna.wsc.messaging.CoordinationContext;

import com.arjuna.mwlabs.wst.at.participants.CompletionCoordinatorImple;
import com.arjuna.mwlabs.wst.at.participants.CompletionWithAckCoordinatorImple;

import javax.naming.InitialContext;

/**
 * This is the interface that allows transactions to be started and terminated.
 * The messaging layer converts the Commit, Rollback and Notify messages into
 * calls on this.
 */

public class UserTransactionImple implements com.arjuna.mw.wst.UserTransaction
{

    public UserTransactionImple ()
    {
	_completionCoordinator = new CompletionCoordinatorImple(_factory.coordinatorManager(), null, null);

	_completionWithAckCoordinator = new CompletionWithAckCoordinatorImple(_factory.coordinatorManager(), null, null);

	_contextManager = ContextManager.contextManager();
	_contextManager.initialise(_factory);
    }
    
    public void begin () throws WrongStateException, SystemException
    {
	begin(0);
    }
    
    public void begin (int timeout) throws WrongStateException, SystemException
    {
	try
	{
	    if (_contextManager.currentTransaction() != null)
		throw new WrongStateException();
	    
	    CoordinationContext ctx = _factory.create(Protocols.AtomicTransaction, null, null);

	    TxContextImple theContext = new TxContextImple(ctx);

	    _contextManager.resume(theContext);
	}
	catch (com.arjuna.wst.UnknownTransactionException ex)
	{
	    ex.printStackTrace();
	    
	    throw new SystemException(ex.toString());
	}
	catch (com.arjuna.wsc.InvalidCreateParametersException ex)
	{
	    throw new SystemException(ex.toString());
	}

	/*
	 * At this point we would normally register a completion participant.
	 *
	 * Registrar reg = _registrarMapper.getRegistrar(_factory.coordinatorManager().identifier().toString());
	 *
	 * reg.register(Protocols.Completion, null);
	 */
    }

    public void commit () throws TransactionRolledBackException, UnknownTransactionException, SystemException
    {
	commit(false);
    }

    public void commit (boolean withAck) throws TransactionRolledBackException, UnknownTransactionException, SystemException
    {
	try
	{
	    if (withAck)
		_completionWithAckCoordinator.commit();
	    else
		_completionCoordinator.commit();
	}
	catch (TransactionRolledBackException ex)
	{
	    throw ex;
	}
	catch (UnknownTransactionException ex)
	{
	    throw ex;
	}
	catch (SystemException ex)
	{
	    throw ex;
	}
	finally
	{
	    if (!withAck)
		_contextManager.tidyup();
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
	    if (withAck)
		_completionWithAckCoordinator.rollback();
	    else
		_completionCoordinator.rollback();
	}
	catch (UnknownTransactionException ex)
	{
	    throw ex;
	}
	catch (SystemException ex)
	{
	    throw ex;
	}
	finally
	{
	    if (!withAck)
		_contextManager.tidyup();
	}
    }

    public void release () throws UnknownTransactionException, SystemException
    {
	_completionWithAckCoordinator.notified();
	_contextManager.tidyup();
    }

    public String transactionIdentifier ()
    {
	try
	{
	    return _factory.coordinatorManager().identifier().toString();
	}
	catch (com.arjuna.mw.wsas.exceptions.NoActivityException ex)
	{
	    return "NoTransaction";
	}
	catch (com.arjuna.mw.wsas.exceptions.SystemException ex)
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

    static LocalContextFactoryImple _factory = new LocalContextFactoryImple();

    private CompletionCoordinatorImple _completionCoordinator = null;
    private CompletionWithAckCoordinatorImple _completionWithAckCoordinator = null;
    private ContextManager _contextManager = null;

}
