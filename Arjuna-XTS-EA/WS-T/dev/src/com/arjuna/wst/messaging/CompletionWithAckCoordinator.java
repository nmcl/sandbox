/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * CompletionWithAckCoordinator.java
 */

package com.arjuna.wst.messaging;

import com.arjuna.wst.messaging.MarshalException;

public interface CompletionWithAckCoordinator
{
    public String getServiceURI();

    public void sendCommitted(CommittedMessage committedMessage)
        throws MarshalException;

    public void sendAborted(AbortedMessage abortedMessage)
        throws MarshalException;

    public void sendError(ErrorMessage errorMessage)
        throws MarshalException;

    public Object recv()
        throws MarshalException;

    public Object recv(String dialogIdentifier)
        throws MarshalException;
}
