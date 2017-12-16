/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * TestNoExceptionOutcomeNotificationParticipant.java
 */

package com.arjuna.wst.tests;

import com.arjuna.wst.SystemException;
import com.arjuna.wst.OutcomeNotificationParticipant;

public class TestNoExceptionOutcomeNotificationParticipant implements OutcomeNotificationParticipant
{
    public void committed()
        throws SystemException
    {
    }

    public void aborted()
        throws SystemException
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
