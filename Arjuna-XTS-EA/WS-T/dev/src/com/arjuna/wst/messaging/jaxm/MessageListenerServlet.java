/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * MessageListenerServlet.java
 */

package com.arjuna.wst.messaging.jaxm;

import javax.naming.InitialContext;
import javax.servlet.ServletException;
import javax.servlet.ServletConfig;
import javax.xml.soap.SOAPMessage;
import javax.xml.messaging.JAXMServlet;
import javax.xml.messaging.OnewayListener;
import com.arjuna.wst.impl.messaging.saaj.MessageListener;

public class MessageListenerServlet extends JAXMServlet implements OnewayListener
{
    public void init(ServletConfig config)
        throws ServletException
    {
        super.init(config);

        try
        {
            InitialContext initialContext = new InitialContext();

            String messageListenerJNDIName = (String) config.getInitParameter("MessageListenerJNDIName");

            _messageListener = (MessageListener) initialContext.lookup(messageListenerJNDIName);

            System.err.println("Message Listener Servlet: started, message listener jndi name \"" + messageListenerJNDIName + "\"");
        }
        catch (Exception exception)
        {
            log("Message Listener Servlet: startup failed", exception);

            throw new ServletException("Message Listener Service: init failed", exception);
        }
        catch (Error error)
        {
            log("Message Listener Servlet: startup failed", error);

            throw new ServletException("Message Listener Servlet: init failed", error);
        }
    }

    public void destroy()
    {
        log("Message Listener Servlet: stopped");

        super.destroy();
    }

    public void onMessage(SOAPMessage message)
    {
        try
        {
            _messageListener.onMessage(message.getSOAPPart().getEnvelope());
        }
        catch (Exception exception)
        {
            log("MessageListenerServlet", exception);
        }
        catch (Error error)
        {
            log("MessageListenerServlet", error);
        }
    }

    private MessageListener _messageListener = null;
}
