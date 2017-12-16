/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * TestNoExceptionPhaseZeroParticipant.java
 */

package com.arjuna.wst.tests;

import com.arjuna.wst.SystemException;
import com.arjuna.wst.PhaseZeroParticipant;

public class TestNoExceptionPhaseZeroParticipant implements PhaseZeroParticipant
{
    public void phaseZero()
        throws SystemException
    {
    }

    public void error()
        throws SystemException
    {
    }
}
