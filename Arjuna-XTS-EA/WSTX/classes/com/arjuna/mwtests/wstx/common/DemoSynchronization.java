/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: DemoSynchronization.java,v 1.1 2002/11/25 11:00:54 nmcl Exp $
 */

package com.arjuna.mwtests.wstx.common;

import com.arjuna.mw.wstx.resource.Synchronization;

import com.arjuna.mw.wscf.common.Qualifier;

import com.arjuna.mw.wstx.common.TxId;

import com.arjuna.mw.wsas.completionstatus.CompletionStatus;

import com.arjuna.mw.wsas.status.Status;

import com.arjuna.mw.wsas.exceptions.SystemException;

/**
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: DemoSynchronization.java,v 1.1 2002/11/25 11:00:54 nmcl Exp $
 * @since 1.0.
 */

public class DemoSynchronization implements Synchronization
{

    public DemoSynchronization (TxId id)
    {
	_tid = id;
    }
    
    public void beforeCompletion () throws SystemException
    {
	System.out.println("DemoSynchronization.beforeCompletion ( "+_tid+" )");
    }
    
    public void afterCompletion (CompletionStatus cs) throws SystemException
    {
	System.out.println("DemoSynchronization.afterCompletion ( "+_tid+", "+cs+" )");
    }

    /**
     * @return the name of this inferior.
     */

    public String name ()
    {
	return "DemoSynchronization";
    }
    
    private TxId _tid;
    
}

