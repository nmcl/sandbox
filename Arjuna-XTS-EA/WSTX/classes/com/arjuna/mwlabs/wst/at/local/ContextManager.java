/*
 * Copyright (C) 2003,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ContextManager.java,v 1.2 2003/02/24 14:35:13 nmcl Exp $
 */

package com.arjuna.mwlabs.wst.at.local;

import com.arjuna.mw.wsas.activity.*;

import com.arjuna.wst.*;

import com.arjuna.mw.wst.common.Protocols;

import com.arjuna.wsc.AlreadyRegisteredException;

import com.arjuna.mw.wst.TxContext;

import javax.naming.InitialContext;

import java.util.*;

/**
 */

public class ContextManager
{
    
    public static final ContextManager contextManager ()
    {
	return _instance;
    }
    
    public final void initialise (LocalContextFactoryImple factory)
    {
	_factory = factory;
    }
    
    public final TxContext suspend () throws SystemException
    {
	ActivityHierarchy wstxHier = null;

	try
	{
	    wstxHier = _factory.coordinatorManager().suspend();
	}
	catch (com.arjuna.mw.wsas.exceptions.SystemException ex)
	{
	    throw new SystemException(ex.toString());
	}

	TxContext txCtx = (TxContext) _threadTxData.remove(Thread.currentThread());
	
	if ((txCtx != null) && (wstxHier != null)) // should be equal!
	{
	    _contextMapper.put(txCtx, wstxHier);
	    
	    return txCtx;
	}
	else
	{
	    if ((txCtx == null) && (wstxHier == null))
		return null;
	    else
	    {
		throw new SystemException("One context was null!");
	    }
	}
    }

    public final TxContext currentTransaction () throws SystemException
    {
	return (TxContext) _threadTxData.get(Thread.currentThread());
    }

    // resume overwrites. Should we check first a la JTA?

    public final void resume (TxContext tx) throws UnknownTransactionException, SystemException
    {
	ActivityHierarchy wstxHier = null;
	boolean imported = true;
	
	if (tx != null)
	{
	    wstxHier = (ActivityHierarchy) _contextMapper.remove(tx);
	
	    if (wstxHier == null)
		imported = false;
	
	    _threadTxData.put(Thread.currentThread(), tx);
	}
	else
	    _threadTxData.remove(Thread.currentThread());
	
	try
	{
	    if (imported)
		_factory.coordinatorManager().resume(wstxHier);
	}
	catch (com.arjuna.mw.wsas.exceptions.InvalidActivityException ex)
	{
	    throw new UnknownTransactionException(ex.toString());
	}
	catch (com.arjuna.mw.wsas.exceptions.SystemException ex)
	{
	    throw new SystemException(ex.toString());
	}
    }

    public final void tidyup ()
    {
	TxContext txCtx = (TxContext) _threadTxData.remove(Thread.currentThread());
	
	if (txCtx != null)
	    _contextMapper.remove(txCtx);
    }

    protected ContextManager ()
    {
    }

    private static WeakHashMap _threadTxData = new WeakHashMap();
    private static HashMap     _contextMapper = new HashMap();

    private static LocalContextFactoryImple _factory = null;

    private static ContextManager _instance = new ContextManager();
    
}
