/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * ActivationCoordinatorDeploymentServlet.java
 */

package com.arjuna.wsc.messaging.deploy.servlet;

import javax.naming.InitialContext;
import javax.servlet.ServletException;
import javax.servlet.ServletConfig;
import javax.servlet.http.HttpServlet;
import com.arjuna.wsc.messaging.ActivationCoordinator;
import com.arjuna.wsc.impl.messaging.saaj.ActivationCoordinatorImpl;

public class ActivationCoordinatorDeploymentServlet extends HttpServlet
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

            ActivationCoordinator activationCoordinator = new ActivationCoordinatorImpl(serviceURI);

            initialContext.bind(jndiName, activationCoordinator);

            log("Activation Coordinator Deployment: done, activation coordinator at \"" + serviceURI + "\", jndi \"" + jndiName + "\"");
        }
        catch (Exception exception)
        {
            log("Activation Coordinator Deployment: failed", exception);

            throw new ServletException("Activation Coordinator Deployment Servlet: init failed", exception);
        }
        catch (Error error)
        {
            log("Activation Coordinator Deployment: failed", error);

            throw new ServletException("Activation Coordinator Deployment Servlet: init failed", error);
        }
    }

    public void destroy()
    {
        log("Activation Coordinator Deployment: undone");

        super.destroy();
    }
}
