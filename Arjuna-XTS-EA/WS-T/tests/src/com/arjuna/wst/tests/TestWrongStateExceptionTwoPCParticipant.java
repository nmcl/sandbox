/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * TestWrongStateExceptionTwoPCParticipant.java
 */

package com.arjuna.wst.tests;

import com.arjuna.wst.SystemException;
import com.arjuna.wst.WrongStateException;
import com.arjuna.wst.TransactionRolledBackException;
import com.arjuna.wst.Vote;
import com.arjuna.wst.TwoPCParticipant;

public class TestWrongStateExceptionTwoPCParticipant implements TwoPCParticipant
{
    public Vote prepare()
        throws WrongStateException, SystemException
    {
        throw new WrongStateException();
    }

    public void commit()
        throws WrongStateException, SystemException
    {
        throw new WrongStateException();
    }

    public void rollback()
        throws WrongStateException, SystemException
    {
        throw new WrongStateException();
    }

    public void commitOnePhase()
        throws TransactionRolledBackException, WrongStateException, SystemException
    {
        throw new WrongStateException();
    }

    public void unknown()
        throws SystemException
    {
        throw new SystemException();
    }

    public void error()
        throws SystemException
    {
        throw new SystemException();
    }
}
