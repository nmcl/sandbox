/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * OutcomeNotificationParticipant.java
 */

package com.arjuna.wst.messaging;

import com.arjuna.wst.messaging.MarshalException;

public interface OutcomeNotificationParticipant
{
    public String getServiceURI();

    public void sendNotified(NotifiedMessage notifiedMessage)
        throws MarshalException;

    public void sendReplay(ReplayMessage replayMessage)
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
