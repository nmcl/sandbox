/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * TwoPCDispatcherDeploymentServlet.java
 */

package com.arjuna.wst.dispatcher.deploy.servlet;

import javax.naming.InitialContext;
import javax.servlet.ServletException;
import javax.servlet.ServletConfig;
import javax.servlet.http.HttpServlet;
import com.arjuna.wst.dispatcher.TwoPCDispatcher;
import com.arjuna.wst.messaging.TwoPCParticipant;

public class TwoPCDispatcherDeploymentServlet extends HttpServlet
{
    public void init(ServletConfig config)
        throws ServletException
    {
        super.init(config);

        try
        {
            InitialContext initialContext = new InitialContext();

            String jndiName                 = config.getInitParameter("JNDIName");
            String twoPCParticipantJNDIName = config.getInitParameter("TwoPCParticipantJNDIName");

            TwoPCParticipant twoPCParticipant = (TwoPCParticipant) initialContext.lookup(twoPCParticipantJNDIName);

            TwoPCDispatcher twoPCDispatcher = new TwoPCDispatcher(twoPCParticipant);

            initialContext.bind(jndiName, twoPCDispatcher);

            log("Two PC Dispatcher Deployment: done, two PC participant at \"" + twoPCParticipantJNDIName + "\", jndi \"" + jndiName + "\"");
        }
        catch (Exception exception)
        {
            log("Two PC Dispatcher Deployment: failed", exception);

            throw new ServletException("Two PC Dispatcher Deployment Servlet: init failed", exception);
        }
        catch (Error error)
        {
            log("Two PC Dispatcher Deployment: failed", error);

            throw new ServletException("Two PC Dispatcher Deployment Servlet: init failed", error);
        }
    }

    public void destroy()
    {
        log("Two PC Dispatcher Deployment: undone");

        super.destroy();
    }
}
