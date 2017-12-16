/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * ReplayMessage.java
 */

package com.arjuna.wst.messaging;

import com.arjuna.wsc.messaging.PortReference;

public class ReplayMessage
{
    public PortReference coordinatorProtocolService = null;
    public PortReference participantProtocolService = null;
    public String        dialogIdentifier           = null;
}
