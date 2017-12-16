/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * PhaseZeroParticipantDeploymentServlet.java
 */

package com.arjuna.wst.messaging.deploy.servlet;

import javax.naming.InitialContext;
import javax.servlet.ServletException;
import javax.servlet.ServletConfig;
import javax.servlet.http.HttpServlet;
import com.arjuna.wst.messaging.PhaseZeroParticipant;
import com.arjuna.wst.impl.messaging.saaj.PhaseZeroParticipantImpl;

public class PhaseZeroParticipantDeploymentServlet extends HttpServlet
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

            PhaseZeroParticipant phaseZeroParticipant = new PhaseZeroParticipantImpl(serviceURI);

            initialContext.bind(jndiName, phaseZeroParticipant);

            log("PhaseZero Participant Deployment: done, phase zero participant at \"" + serviceURI + "\", jndi \"" + jndiName + "\"");
        }
        catch (Exception exception)
        {
            log("PhaseZero Participant Deployment: failed", exception);

            throw new ServletException("PhaseZero Participant Deployment Servlet: init failed", exception);
        }
        catch (Error error)
        {
            log("PhaseZero Participant Deployment: failed", error);

            throw new ServletException("PhaseZero Participant Deployment Servlet: init failed", error);
        }
    }

    public void destroy()
    {
        log("PhaseZero Participant Deployment: undone");

        super.destroy();
    }
}
