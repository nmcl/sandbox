/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * OutcomeNotificationParticipantDeploymentServlet.java
 */

package com.arjuna.wst.messaging.deploy.servlet;

import javax.naming.InitialContext;
import javax.servlet.ServletException;
import javax.servlet.ServletConfig;
import javax.servlet.http.HttpServlet;
import com.arjuna.wst.messaging.OutcomeNotificationParticipant;
import com.arjuna.wst.impl.messaging.saaj.OutcomeNotificationParticipantImpl;

public class OutcomeNotificationParticipantDeploymentServlet extends HttpServlet
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

            OutcomeNotificationParticipant outcomeNotificationParticipant = new OutcomeNotificationParticipantImpl(serviceURI);

            initialContext.bind(jndiName, outcomeNotificationParticipant);

            log("OutcomeNotification Participant Deployment: done, outcome notification participant at \"" + serviceURI + "\", jndi \"" + jndiName + "\"");
        }
        catch (Exception exception)
        {
            log("OutcomeNotification Participant Deployment: failed", exception);

            throw new ServletException("OutcomeNotification Participant Deployment Servlet: init failed", exception);
        }
        catch (Error error)
        {
            log("OutcomeNotification Participant Deployment: failed", error);

            throw new ServletException("OutcomeNotification Participant Deployment Servlet: init failed", error);
        }
    }

    public void destroy()
    {
        log("OutcomeNotification Participant Deployment: undone");

        super.destroy();
    }
}
