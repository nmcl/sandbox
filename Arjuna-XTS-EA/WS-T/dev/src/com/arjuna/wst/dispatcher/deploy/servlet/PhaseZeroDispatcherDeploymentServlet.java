/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * PhaseZeroDispatcherDeploymentServlet.java
 */

package com.arjuna.wst.dispatcher.deploy.servlet;

import javax.naming.InitialContext;
import javax.servlet.ServletException;
import javax.servlet.ServletConfig;
import javax.servlet.http.HttpServlet;
import com.arjuna.wst.dispatcher.PhaseZeroDispatcher;
import com.arjuna.wst.messaging.PhaseZeroParticipant;

public class PhaseZeroDispatcherDeploymentServlet extends HttpServlet
{
    public void init(ServletConfig config)
        throws ServletException
    {
        super.init(config);

        try
        {
            InitialContext initialContext = new InitialContext();

            String jndiName                     = config.getInitParameter("JNDIName");
            String phaseZeroParticipantJNDIName = config.getInitParameter("PhaseZeroParticipantJNDIName");

            PhaseZeroParticipant phaseZeroParticipant = (PhaseZeroParticipant) initialContext.lookup(phaseZeroParticipantJNDIName);

            PhaseZeroDispatcher phaseZeroDispatcher = new PhaseZeroDispatcher(phaseZeroParticipant);

            initialContext.bind(jndiName, phaseZeroDispatcher);

            log("Phase Zero Dispatcher Deployment: done, phase zero participant at \"" + phaseZeroParticipantJNDIName + "\", jndi \"" + jndiName + "\"");
        }
        catch (Exception exception)
        {
            log("Phase Zero Dispatcher Deployment: failed", exception);

            throw new ServletException("Phase Zero Dispatcher Deployment Servlet: init failed", exception);
        }
        catch (Error error)
        {
            log("Phase Zero Dispatcher Deployment: failed", error);

            throw new ServletException("Phase Zero Dispatcher Deployment Servlet: init failed", error);
        }
    }

    public void destroy()
    {
        log("Phase Zero Dispatcher Deployment: undone");

        super.destroy();
    }
}
