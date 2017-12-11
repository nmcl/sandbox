/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * ActivationCoordinator.java
 */

package com.arjuna.wsc.messaging;

import com.arjuna.wsc.messaging.MarshalException;

public interface ActivationCoordinator
{
    public String getServiceURI();

    public CreateCoordinationContextMessage recvCreateCoordinationContext()
        throws MarshalException;

    public void sendCreateCoordinationContextResponse(CreateCoordinationContextResponseMessage createCoordinationContextResponseMessage)
        throws MarshalException;

    public void sendError(ErrorMessage errorMessage)
        throws MarshalException;
}
