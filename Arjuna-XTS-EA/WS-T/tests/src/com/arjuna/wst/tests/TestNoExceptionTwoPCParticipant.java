/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * TestNoExceptionTwoPCParticipant.java
 */

package com.arjuna.wst.tests;

import com.arjuna.wst.SystemException;
import com.arjuna.wst.WrongStateException;
import com.arjuna.wst.TransactionRolledBackException;
import com.arjuna.wst.Vote;
import com.arjuna.wst.TwoPCParticipant;

public class TestNoExceptionTwoPCParticipant implements TwoPCParticipant
{
    public Vote prepare()
        throws WrongStateException, SystemException
    {
        throw new SystemException();
    }

    public void commit()
        throws WrongStateException, SystemException
    {
    }

    public void rollback()
        throws WrongStateException, SystemException
    {
    }

    public void commitOnePhase()
        throws TransactionRolledBackException, WrongStateException, SystemException
    {
    }

    public void unknown()
        throws SystemException
    {
    }

    public void error()
        throws SystemException
    {
    }
}
