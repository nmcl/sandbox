/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * RegisterMessage.java
 */

package com.arjuna.wsc.messaging;

public class RegisterMessage
{
    public PortReference registrationService        = null;
    public PortReference requesterReference         = null;
    public String        protocolIdentifier         = null;
    public PortReference participantProtocolService = null;
    public String        dialogIdentifier           = null;
}
