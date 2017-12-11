/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * ErrorMessage.java
 */

package com.arjuna.wsc.messaging;

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
