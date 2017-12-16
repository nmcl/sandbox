/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * CompletionCoordinatorDeploymentServlet.java
 */

package com.arjuna.wst.messaging.deploy.servlet;

import javax.naming.InitialContext;
import javax.servlet.ServletException;
import javax.servlet.ServletConfig;
import javax.servlet.http.HttpServlet;
import com.arjuna.wst.messaging.CompletionCoordinator;
import com.arjuna.wst.impl.messaging.saaj.CompletionCoordinatorImpl;

public class CompletionCoordinatorDeploymentServlet extends HttpServlet
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

            CompletionCoordinator completionCoordinator = new CompletionCoordinatorImpl(serviceURI);

            initialContext.bind(jndiName, completionCoordinator);

            log("Completion Coordinator Deployment: done, completion coordinator at \"" + serviceURI + "\", jndi \"" + jndiName + "\"");
        }
        catch (Exception exception)
        {
            log("Completion Coordinator Deployment: failed", exception);

            throw new ServletException("Completion Coordinator Deployment Servlet: init failed", exception);
        }
        catch (Error error)
        {
            log("Completion Coordinator Deployment: failed", error);

            throw new ServletException("Completion Coordinator Deployment Servlet: init failed", error);
        }
    }

    public void destroy()
    {
        log("Completion Coordinator Deployment: undone");

        super.destroy();
    }
}
