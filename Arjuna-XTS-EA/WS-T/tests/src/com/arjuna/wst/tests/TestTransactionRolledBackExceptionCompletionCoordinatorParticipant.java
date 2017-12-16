/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * TestTransactionRolledBackExceptionCompletionCoordinatorParticipant.java
 */

package com.arjuna.wst.tests;

import com.arjuna.wst.SystemException;
import com.arjuna.wst.TransactionRolledBackException;
import com.arjuna.wst.UnknownTransactionException;
import com.arjuna.wst.CompletionCoordinatorParticipant;

public class TestTransactionRolledBackExceptionCompletionCoordinatorParticipant implements CompletionCoordinatorParticipant
{
    public void commit()
        throws TransactionRolledBackException, UnknownTransactionException, SystemException
    {
        throw new TransactionRolledBackException();
    }

    public void rollback()
        throws UnknownTransactionException, SystemException
    {
        throw new SystemException();
    }
}
