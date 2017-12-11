/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * RegisterResponseMessage.java
 */

package com.arjuna.wsc.messaging;

public class RegisterResponseMessage
{
    public PortReference requesterReference          = null;
    public PortReference coordinationProtocolService = null;
    public String        dialogIdentifier            = null;
}
