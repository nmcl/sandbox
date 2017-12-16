/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * PhaseZeroCoordinator.java
 */

package com.arjuna.wst.messaging;

import com.arjuna.wst.messaging.MarshalException;

public interface PhaseZeroCoordinator
{
    public String getServiceURI();

    public void sendPhaseZero(PhaseZeroMessage phaseZeroMessage)
        throws MarshalException;

    public void sendError(ErrorMessage errorMessage)
        throws MarshalException;

    public Object recv()
        throws MarshalException;

    public Object recv(String dialogIdentifier)
        throws MarshalException;
}
