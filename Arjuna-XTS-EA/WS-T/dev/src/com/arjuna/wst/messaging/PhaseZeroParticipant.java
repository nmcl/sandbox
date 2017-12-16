/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * PhaseZeroParticipant.java
 */

package com.arjuna.wst.messaging;

import com.arjuna.wst.messaging.MarshalException;

public interface PhaseZeroParticipant
{
    public String getServiceURI();

    public void sendPhaseZeroCompleted(PhaseZeroCompletedMessage phaseZeroCompletedMessage)
        throws MarshalException;

    public void sendUnknown(UnknownMessage unknownMessage)
        throws MarshalException;

    public void sendError(ErrorMessage errorMessage)
        throws MarshalException;

    public Object recv()
        throws MarshalException;

    public Object recv(String dialogIdentifier)
        throws MarshalException;
}
