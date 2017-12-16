/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * TestSystemExceptionPhaseZeroParticipant.java
 */

package com.arjuna.wst.tests;

import com.arjuna.wst.SystemException;
import com.arjuna.wst.PhaseZeroParticipant;

public class TestSystemExceptionPhaseZeroParticipant implements PhaseZeroParticipant
{
    public void phaseZero()
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
