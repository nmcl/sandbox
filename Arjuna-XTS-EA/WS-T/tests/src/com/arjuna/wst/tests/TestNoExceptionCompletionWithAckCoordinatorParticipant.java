/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * TestNoExceptionCompletionWithAckCoordinatorParticipant.java
 */

package com.arjuna.wst.tests;

import com.arjuna.wst.SystemException;
import com.arjuna.wst.TransactionRolledBackException;
import com.arjuna.wst.UnknownTransactionException;
import com.arjuna.wst.CompletionWithAckCoordinatorParticipant;

public class TestNoExceptionCompletionWithAckCoordinatorParticipant implements CompletionWithAckCoordinatorParticipant
{
    public void commit()
        throws TransactionRolledBackException, UnknownTransactionException, SystemException
    {
    }

    public void rollback()
        throws UnknownTransactionException, SystemException
    {
    }

    public void notified()
        throws UnknownTransactionException, SystemException
    {
    }
}