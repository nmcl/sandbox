/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * MessageListenerService.java
 */

package com.arjuna.wst.messaging.axis;

import javax.naming.InitialContext;
import javax.xml.rpc.ServiceException;
import javax.xml.rpc.handler.MessageContext;
import javax.xml.rpc.server.ServiceLifecycle;
import javax.xml.rpc.server.ServletEndpointContext;
import com.arjuna.wst.impl.messaging.saaj.MessageListener;

public class MessageListenerService implements ServiceLifecycle
{
    public void init(Object context)
        throws ServiceException
    {
        System.err.println("MessageListenerService: init");

        try
        {
            InitialContext initialContext = new InitialContext();
            MessageContext messageContext = ((ServletEndpointContext) context).getMessageContext();

            String messageListenerJNDIName = (String) messageContext.getProperty("MessageListenerJNDIName");

            _messageListener = (MessageListener) initialContext.lookup(messageListenerJNDIName);

            System.err.println("Message Listener Service: started, message listener jndi name \"" + messageListenerJNDIName + "\"");
        }
        catch (Exception exception)
        {
            System.err.println("Message Listener Service: startup failed: " + exception);
            exception.printStackTrace(System.err);

            throw new ServiceException("Message Listener Service: init failed", exception);
        }
        catch (Error error)
        {
            System.err.println("Message Listener Service: startup failed: " + error);
            error.printStackTrace(System.err);

            throw new ServiceException("Message Listener Service: init failed", error);
        }
    }

    public void destroy()
    {
        System.err.println("Message Listener Service: stopped");
    }

    public void onMessage(org.apache.axis.message.SOAPEnvelope request, org.apache.axis.message.SOAPEnvelope response)
    {
        try
        {
            _messageListener.onMessage(request);
        }
        catch (Exception exception)
        {
            System.err.println("MessageListenerService: " + exception);
        }
        catch (Error error)
        {
            System.err.println("MessageListenerService: " + error);
        }
    }

    private MessageListener _messageListener = null;
}
