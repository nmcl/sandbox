/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * CoordinationContext.java
 */

package com.arjuna.wsc.messaging;

public class CoordinationContext
{
    public String             identifier          = null;
    public AttributedDateTime expires             = null; // Can be null.
    public String             coordinationType    = null;
    public PortReference      registrationService = null;
}
