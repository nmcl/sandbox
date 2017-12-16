/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * OutcomeNotificationDispatcherDeploymentServlet.java
 */

package com.arjuna.wst.dispatcher.deploy.servlet;

import javax.naming.InitialContext;
import javax.servlet.ServletException;
import javax.servlet.ServletConfig;
import javax.servlet.http.HttpServlet;
import com.arjuna.wst.dispatcher.OutcomeNotificationDispatcher;
import com.arjuna.wst.messaging.OutcomeNotificationParticipant;

public class OutcomeNotificationDispatcherDeploymentServlet extends HttpServlet
{
    public void init(ServletConfig config)
        throws ServletException
    {
        super.init(config);

        try
        {
            InitialContext initialContext = new InitialContext();

            String jndiName                               = config.getInitParameter("JNDIName");
            String outcomeNotificationParticipantJNDIName = config.getInitParameter("OutcomeNotificationParticipantJNDIName");

            OutcomeNotificationParticipant outcomeNotificationParticipant = (OutcomeNotificationParticipant) initialContext.lookup(outcomeNotificationParticipantJNDIName);

            OutcomeNotificationDispatcher outcomeNotificationDispatcher = new OutcomeNotificationDispatcher(outcomeNotificationParticipant);

            initialContext.bind(jndiName, outcomeNotificationDispatcher);

            log("Outcome Notification Dispatcher Deployment: done, outcome notification participant at \"" + outcomeNotificationParticipantJNDIName + "\", jndi \"" + jndiName + "\"");
        }
        catch (Exception exception)
        {
            log("Outcome Notification Dispatcher Deployment: failed", exception);

            throw new ServletException("Outcome Notification Dispatcher Deployment Servlet: init failed", exception);
        }
        catch (Error error)
        {
            log("Outcome Notification Dispatcher Deployment: failed", error);

            throw new ServletException("Outcome Notification Dispatcher Deployment Servlet: init failed", error);
        }
    }

    public void destroy()
    {
        log("Outcome Notification Dispatcher Deployment: undone");

        super.destroy();
    }
}
