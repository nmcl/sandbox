/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * OutcomeNotificationCoordinatorDeploymentServlet.java
 */

package com.arjuna.wst.messaging.deploy.servlet;

import javax.naming.InitialContext;
import javax.servlet.ServletException;
import javax.servlet.ServletConfig;
import javax.servlet.http.HttpServlet;
import com.arjuna.wst.messaging.OutcomeNotificationCoordinator;
import com.arjuna.wst.impl.messaging.saaj.OutcomeNotificationCoordinatorImpl;

public class OutcomeNotificationCoordinatorDeploymentServlet extends HttpServlet
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

            OutcomeNotificationCoordinator outcomeNotificationCoordinator = new OutcomeNotificationCoordinatorImpl(serviceURI);

            initialContext.bind(jndiName, outcomeNotificationCoordinator);

            log("OutcomeNotification Coordinator Deployment: done, outcome notification coordinator at \"" + serviceURI + "\", jndi \"" + jndiName + "\"");
        }
        catch (Exception exception)
        {
            log("OutcomeNotification Coordinator Deployment: failed", exception);

            throw new ServletException("OutcomeNotification Coordinator Deployment Servlet: init failed", exception);
        }
        catch (Error error)
        {
            log("OutcomeNotification Coordinator Deployment: failed", error);

            throw new ServletException("OutcomeNotification Coordinator Deployment Servlet: init failed", error);
        }
    }

    public void destroy()
    {
        log("OutcomeNotification Coordinator Deployment: undone");

        super.destroy();
    }
}
