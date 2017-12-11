/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * MessageListener.java
 */

package com.arjuna.wsc.impl.messaging.saaj;

import javax.xml.soap.SOAPEnvelope;

public interface MessageListener
{
    public void onMessage(SOAPEnvelope envelope);
}
