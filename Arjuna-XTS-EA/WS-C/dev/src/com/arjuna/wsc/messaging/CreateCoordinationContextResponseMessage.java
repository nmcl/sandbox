/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * CreateCoordinationContextResponseMessage.java
 */

package com.arjuna.wsc.messaging;

public class CreateCoordinationContextResponseMessage
{
    public PortReference       requesterReference  = null;
    public CoordinationContext coordinationContext = null; // Can be null.
    public String              dialogIdentifier    = null;
}
