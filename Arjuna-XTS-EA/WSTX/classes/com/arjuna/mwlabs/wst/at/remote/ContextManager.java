/*
 * Copyright (C) 2003,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ContextManager.java,v 1.1 2003/02/19 16:13:04 nmcl Exp $
 */

package com.arjuna.mwlabs.wst.at.remote;

import com.arjuna.mwlabs.wsc.*;

import com.arjuna.mwlabs.wst.at.context.*;

import com.arjuna.mw.wst.common.Protocols;

import com.arjuna.wst.*;

import com.arjuna.mw.wst.TxContext;

import javax.naming.InitialContext;

import java.util.*;

/**
 */

// publish via JNDI for each address space?

public class ContextManager
{

    public ContextManager ()
    {
    }
    
    // resume overwrites. Should we check first a la JTA?

    public void resume (TxContext tx) throws UnknownTransactionException, SystemException
    {
	if (tx != null)
	{
	    _threadTxData.put(Thread.currentThread(), tx);
	}
	else
	    _threadTxData.remove(Thread.currentThread());
    }
    
    public TxContext suspend () throws SystemException
    {
	return (TxContext) _threadTxData.remove(Thread.currentThread());
    }

    public TxContext currentTransaction () throws SystemException
    {
	return (TxContext) _threadTxData.get(Thread.currentThread());
    }
    
    private static WeakHashMap _threadTxData = new WeakHashMap();
    
}
