/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * MessageListener.java
 */

package com.arjuna.wst.impl.messaging.saaj;

import javax.xml.soap.SOAPEnvelope;

public interface MessageListener
{
    public void onMessage(SOAPEnvelope envelope);
}
