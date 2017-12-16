/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * CompletionWithAckParticipantDeploymentServlet.java
 */

package com.arjuna.wst.messaging.deploy.servlet;

import javax.naming.InitialContext;
import javax.servlet.ServletException;
import javax.servlet.ServletConfig;
import javax.servlet.http.HttpServlet;
import com.arjuna.wst.messaging.CompletionWithAckParticipant;
import com.arjuna.wst.impl.messaging.saaj.CompletionWithAckParticipantImpl;

public class CompletionWithAckParticipantDeploymentServlet extends HttpServlet
{
    public void init(ServletConfig config)
        throws ServletException
    {
        super.init(config);

        try
        {
            InitialContext initialContext = new InitialContext();

            String jndiName   = config.getInitParameter("JNDIName");
            String serviceURI = config.getInitParameter("ServiceURI");

            CompletionWithAckParticipant completionWithAckParticipant = new CompletionWithAckParticipantImpl(serviceURI);

            initialContext.bind(jndiName, completionWithAckParticipant);

            log("CompletionWithAck Participant Deployment: done, completion with ack participant at \"" + serviceURI + "\", jndi \"" + jndiName + "\"");
        }
        catch (Exception exception)
        {
            log("CompletionWithAck Participant Deployment: failed", exception);

            throw new ServletException("CompletionWithAck Participant Deployment Servlet: init failed", exception);
        }
        catch (Error error)
        {
            log("CompletionWithAck Participant Deployment: failed", error);

            throw new ServletException("CompletionWithAck Participant Deployment Servlet: init failed", error);
        }
    }

    public void destroy()
    {
        log("CompletionWithAck Participant Deployment: undone");

        super.destroy();
    }
}
