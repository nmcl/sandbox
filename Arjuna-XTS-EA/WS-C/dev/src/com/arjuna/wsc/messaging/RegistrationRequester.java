/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * RegistrationRequester.java
 */

package com.arjuna.wsc.messaging;

import com.arjuna.wsc.messaging.MarshalException;

public interface RegistrationRequester
{
    public String getServiceURI();

    public void sendRegister(RegisterMessage registerMessage)
        throws MarshalException;

    public RegisterResponseMessage recvRegisterResponse(String dialogIdentifier)
        throws MarshalException, ErrorMessageException;
}
