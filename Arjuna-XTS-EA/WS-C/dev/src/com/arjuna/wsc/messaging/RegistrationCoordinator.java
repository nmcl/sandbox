/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * RegistrationCoordinator.java
 */

package com.arjuna.wsc.messaging;

import com.arjuna.wsc.messaging.MarshalException;

public interface RegistrationCoordinator
{
    public String getServiceURI();

    public RegisterMessage recvRegister()
        throws MarshalException;

    public void sendRegisterResponse(RegisterResponseMessage registerResponseMessage)
        throws MarshalException;

    public void sendError(ErrorMessage errorMessage)
        throws MarshalException;
}
