/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * RegistrationCoordinatorDeploymentServlet.java
 */

package com.arjuna.wsc.messaging.deploy.servlet;

import javax.naming.InitialContext;
import javax.servlet.ServletException;
import javax.servlet.ServletConfig;
import javax.servlet.http.HttpServlet;
import com.arjuna.wsc.messaging.RegistrationCoordinator;
import com.arjuna.wsc.impl.messaging.saaj.RegistrationCoordinatorImpl;

public class RegistrationCoordinatorDeploymentServlet extends HttpServlet
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

            RegistrationCoordinator registrationCoordinator = new RegistrationCoordinatorImpl(serviceURI);

            initialContext.bind(jndiName, registrationCoordinator);

            log("Registration Coordinator Deployment: done, registration coordinator at \"" + serviceURI + "\", jndi \"" + jndiName + "\"");
        }
        catch (Exception exception)
        {
            log("Registration Coordinator Deployment: failed", exception);

            throw new ServletException("Registration Coordinator Deployment Servlet: init failed", exception);
        }
        catch (Error error)
        {
            log("Registration Coordinator Deployment: failed", error);

            throw new ServletException("Registration Coordinator Deployment Servlet: init failed", error);
        }
    }

    public void destroy()
    {
        log("Registration Coordinator Deployment: undone");

        super.destroy();
    }
}
