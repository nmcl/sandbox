/*
 * Copyright (C) 2003,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: TransactionManagerImple.java,v 1.10 2003/04/04 15:00:56 nmcl Exp $
 */

package com.arjuna.mwlabs.wst.at.local;

import com.arjuna.wst.*;

import com.arjuna.mw.wst.common.Protocols;

import com.arjuna.wsc.AlreadyRegisteredException;

import com.arjuna.mw.wst.TxContext;

import javax.naming.InitialContext;

import java.util.*;

/**
 * This is the interface that the core exposes in order to allow different
 * types of participants to be enrolled. The messaging layer continues to
 * work in terms of the registrar, but internally we map to one of these
 * methods.
 *
 * This could also be the interface that high-level users see (e.g., at the
 * application Web Service).
 */

public class TransactionManagerImple implements com.arjuna.mw.wst.TransactionManager
{

    public TransactionManagerImple ()
    {
	_contextManager = ContextManager.contextManager();
	_contextManager.initialise(UserTransactionImple._factory);
    }
	
    public void enlistForPhaseZero (PhaseZeroParticipant pzp, String id) throws WrongStateException, UnknownTransactionException, AlreadyRegisteredException, SystemException
    {
	try
	{
	    _registrar.register(pzp, Protocols.PhaseZero);
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

    public void enlistForTwoPhase (TwoPCParticipant tpp, String id) throws WrongStateException, UnknownTransactionException, AlreadyRegisteredException, SystemException
    {
	try
	{
	    _registrar.register(tpp, Protocols.TwoPhaseCommit);
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

    public void enlistForOutcomeNotification (OutcomeNotificationParticipant onp, String id) throws WrongStateException, UnknownTransactionException, AlreadyRegisteredException, SystemException
    {
	try
	{
	    _registrar.register(onp, Protocols.OutcomeNotification);
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

    public void resume (TxContext tx) throws UnknownTransactionException, SystemException
    {
	_contextManager.resume(tx);
    }
    
    public TxContext suspend () throws SystemException
    {
	return _contextManager.suspend();
    }

    public TxContext currentTransaction () throws SystemException
    {
	return _contextManager.currentTransaction();
    }

    final LocalRegistrarImple getRegistrar ()
    {
	return _registrar;
    }

    private ContextManager _contextManager = null;
    
    private static LocalRegistrarImple _registrar = new LocalRegistrarImple();
    
}
