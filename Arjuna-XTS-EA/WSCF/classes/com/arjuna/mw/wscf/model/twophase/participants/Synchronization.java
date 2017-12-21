/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Synchronization.java,v 1.1 2003/01/07 10:33:45 nmcl Exp $
 */

package com.arjuna.mw.wscf.model.twophase.participants;

import com.arjuna.mw.wscf.common.Qualifier;

import com.arjuna.mw.wsas.exceptions.SystemException;

/**
 * This is the interface that all synchronization participants must define.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: Synchronization.java,v 1.1 2003/01/07 10:33:45 nmcl Exp $
 * @since 1.0.
 */

public interface Synchronization
{

    /**
     * The transaction that the instance is enrolled with is about to
     * commit.
     * 
     * @exception SystemException Thrown if any error occurs. This will cause
     * the transaction to roll back.
     */

    public void beforeCompletion () throws SystemException;

    /**
     * The transaction that the instance is enrolled with has completed and
     * the state in which is completed is passed as a parameter.
     *
     * @param CompletionStatus cs The state in which the transaction completed.
     *
     * @exception SystemException Thrown if any error occurs. This has no
     * affect on the outcome of the transaction.
     */

    public void afterCompletion (int status) throws SystemException;
    
}

