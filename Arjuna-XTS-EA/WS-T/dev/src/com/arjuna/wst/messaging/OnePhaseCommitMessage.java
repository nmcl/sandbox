/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * OnePhaseCommitMessage.java
 */

package com.arjuna.wst.messaging;

import com.arjuna.wsc.messaging.PortReference;

public class OnePhaseCommitMessage
{
    public PortReference targetProtocolService = null;
    public PortReference sourceProtocolService = null;
    public String        dialogIdentifier      = null;
}
