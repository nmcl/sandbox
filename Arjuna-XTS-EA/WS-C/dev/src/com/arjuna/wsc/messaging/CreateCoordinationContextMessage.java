/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * CreateCoordinationContextMessage.java
 */

package com.arjuna.wsc.messaging;

public class CreateCoordinationContextMessage
{
    public PortReference       activationService  = null;
    public AttributedDateTime  expires            = null; // Can be null.
    public CoordinationContext currentContext     = null; // Can be null.
    public PortReference       requesterReference = null;
    public String              coordinationType   = null;
    public String              dialogIdentifier   = null;
}
