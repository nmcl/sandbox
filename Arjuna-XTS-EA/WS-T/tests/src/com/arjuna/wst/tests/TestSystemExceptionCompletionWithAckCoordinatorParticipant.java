/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * TestSystemExceptionCompletionWithAckCoordinatorParticipant.java
 */

package com.arjuna.wst.tests;

import com.arjuna.wst.SystemException;
import com.arjuna.wst.TransactionRolledBackException;
import com.arjuna.wst.UnknownTransactionException;
import com.arjuna.wst.CompletionWithAckCoordinatorParticipant;

public class TestSystemExceptionCompletionWithAckCoordinatorParticipant implements CompletionWithAckCoordinatorParticipant
{
    public void commit()
        throws TransactionRolledBackException, UnknownTransactionException, SystemException
    {
        throw new SystemException();
    }

    public void rollback()
        throws UnknownTransactionException, SystemException
    {
        throw new SystemException();
    }

    public void notified()
        throws UnknownTransactionException, SystemException
    {
        throw new SystemException();
    }
}
