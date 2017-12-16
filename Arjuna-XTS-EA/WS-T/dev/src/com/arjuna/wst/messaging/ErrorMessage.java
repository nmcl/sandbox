/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * ErrorMessage.java
 */

package com.arjuna.wst.messaging;

import com.arjuna.wsc.messaging.PortReference;
import com.arjuna.wst.messaging.ErrorCode;

public class ErrorMessage
{
    public PortReference targetProtocolService = null;
    public ErrorCode     errorCode             = null;
    public String        dialogIdentifier      = null;

    public String toString()
    {
        return "targetProtocolServiceAddress=" + targetProtocolService.getAddress() + ",errorCode=[" + errorCode + "]";
    }
}
