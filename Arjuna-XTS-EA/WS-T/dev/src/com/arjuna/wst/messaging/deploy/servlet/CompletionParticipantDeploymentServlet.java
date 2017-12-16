/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * CompletionParticipantDeploymentServlet.java
 */

package com.arjuna.wst.messaging.deploy.servlet;

import javax.naming.InitialContext;
import javax.servlet.ServletException;
import javax.servlet.ServletConfig;
import javax.servlet.http.HttpServlet;
import com.arjuna.wst.messaging.CompletionParticipant;
import com.arjuna.wst.impl.messaging.saaj.CompletionParticipantImpl;

public class CompletionParticipantDeploymentServlet extends HttpServlet
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

            CompletionParticipant completionParticipant = new CompletionParticipantImpl(serviceURI);

            initialContext.bind(jndiName, completionParticipant);

            log("Completion Participant Deployment: done, completion participant at \"" + serviceURI + "\", jndi \"" + jndiName + "\"");
        }
        catch (Exception exception)
        {
            log("Completion Participant Deployment: failed", exception);

            throw new ServletException("Completion Participant Deployment Servlet: init failed", exception);
        }
        catch (Error error)
        {
            log("Completion Participant Deployment: failed", error);

            throw new ServletException("Completion Participant Deployment Servlet: init failed", error);
        }
    }

    public void destroy()
    {
        log("Completion Participant Deployment: undone");

        super.destroy();
    }
}
