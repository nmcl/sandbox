/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * TwoPCParticipantDeploymentServlet.java
 */

package com.arjuna.wst.messaging.deploy.servlet;

import javax.naming.InitialContext;
import javax.servlet.ServletException;
import javax.servlet.ServletConfig;
import javax.servlet.http.HttpServlet;
import com.arjuna.wst.messaging.TwoPCParticipant;
import com.arjuna.wst.impl.messaging.saaj.TwoPCParticipantImpl;

public class TwoPCParticipantDeploymentServlet extends HttpServlet
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

            TwoPCParticipant twoPCParticipant = new TwoPCParticipantImpl(serviceURI);

            initialContext.bind(jndiName, twoPCParticipant);

            log("TwoPC Participant Deployment: done, two pc participant at \"" + serviceURI + "\", jndi \"" + jndiName + "\"");
        }
        catch (Exception exception)
        {
            log("TwoPC Participant Deployment: failed", exception);

            throw new ServletException("TwoPC Participant Deployment Servlet: init failed", exception);
        }
        catch (Error error)
        {
            log("TwoPC Participant Deployment: failed", error);

            throw new ServletException("TwoPC Participant Deployment Servlet: init failed", error);
        }
    }

    public void destroy()
    {
        log("TwoPC Participant Deployment: undone");

        super.destroy();
    }
}
