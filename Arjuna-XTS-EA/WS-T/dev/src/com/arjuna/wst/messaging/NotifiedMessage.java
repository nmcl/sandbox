/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * NotifiedMessage.java
 */

package com.arjuna.wst.messaging;

import com.arjuna.wsc.messaging.PortReference;

public class NotifiedMessage
{
    public PortReference targetProtocolService = null;
    public PortReference sourceProtocolService = null;
    public String        dialogIdentifier      = null;
}
