/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * TestSystemExceptionOutcomeNotificationParticipant.java
 */

package com.arjuna.wst.tests;

import com.arjuna.wst.SystemException;
import com.arjuna.wst.OutcomeNotificationParticipant;

public class TestSystemExceptionOutcomeNotificationParticipant implements OutcomeNotificationParticipant
{
    public void committed()
        throws SystemException
    {
        throw new SystemException();
    }

    public void aborted()
        throws SystemException
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
