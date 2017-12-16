/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * TestPreparedVoteTwoPCParticipant.java
 */

package com.arjuna.wst.tests;

import com.arjuna.wst.SystemException;
import com.arjuna.wst.WrongStateException;
import com.arjuna.wst.TransactionRolledBackException;
import com.arjuna.wst.Vote;
import com.arjuna.wst.Prepared;
import com.arjuna.wst.TwoPCParticipant;

public class TestPreparedVoteTwoPCParticipant implements TwoPCParticipant
{
    public Vote prepare()
        throws WrongStateException, SystemException
    {
        return new Prepared();
    }

    public void commit()
        throws WrongStateException, SystemException
    {
        throw new SystemException();
    }

    public void rollback()
        throws WrongStateException, SystemException
    {
        throw new SystemException();
    }

    public void commitOnePhase()
        throws TransactionRolledBackException, WrongStateException, SystemException
    {
        throw new SystemException();
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
