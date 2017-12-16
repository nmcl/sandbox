/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * TestUnknownTransactionExceptionCompletionCoordinatorParticipant.java
 */

package com.arjuna.wst.tests;

import com.arjuna.wst.SystemException;
import com.arjuna.wst.TransactionRolledBackException;
import com.arjuna.wst.UnknownTransactionException;
import com.arjuna.wst.CompletionCoordinatorParticipant;

public class TestUnknownTransactionExceptionCompletionCoordinatorParticipant implements CompletionCoordinatorParticipant
{
    public void commit()
        throws TransactionRolledBackException, UnknownTransactionException, SystemException
    {
        throw new UnknownTransactionException();
    }

    public void rollback()
        throws UnknownTransactionException, SystemException
    {
        throw new UnknownTransactionException();
    }
}
