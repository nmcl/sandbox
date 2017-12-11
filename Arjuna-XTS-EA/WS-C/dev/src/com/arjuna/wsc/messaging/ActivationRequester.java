/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * ActivationRequester.java
 */

package com.arjuna.wsc.messaging;

import com.arjuna.wsc.messaging.MarshalException;

public interface ActivationRequester
{
    public String getServiceURI();

    public void sendCreateCoordinationContext(CreateCoordinationContextMessage createCoordinationContextMessage)
        throws MarshalException;

    public CreateCoordinationContextResponseMessage recvCreateCoordinationContextResponse(String dialogIdentifier)
        throws MarshalException, ErrorMessageException;
}
