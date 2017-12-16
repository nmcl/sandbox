/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * PhaseZeroCoordinatorDeploymentServlet.java
 */

package com.arjuna.wst.messaging.deploy.servlet;

import javax.naming.InitialContext;
import javax.servlet.ServletException;
import javax.servlet.ServletConfig;
import javax.servlet.http.HttpServlet;
import com.arjuna.wst.messaging.PhaseZeroCoordinator;
import com.arjuna.wst.impl.messaging.saaj.PhaseZeroCoordinatorImpl;

public class PhaseZeroCoordinatorDeploymentServlet extends HttpServlet
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

            PhaseZeroCoordinator phaseZeroCoordinator = new PhaseZeroCoordinatorImpl(serviceURI);

            initialContext.bind(jndiName, phaseZeroCoordinator);

            log("PhaseZero Coordinator Deployment: done, phase zero coordinator at \"" + serviceURI + "\", jndi \"" + jndiName + "\"");
        }
        catch (Exception exception)
        {
            log("PhaseZero Coordinator Deployment: failed", exception);

            throw new ServletException("PhaseZero Coordinator Deployment Servlet: init failed", exception);
        }
        catch (Error error)
        {
            log("PhaseZero Coordinator Deployment: failed", error);

            throw new ServletException("PhaseZero Coordinator Deployment Servlet: init failed", error);
        }
    }

    public void destroy()
    {
        log("PhaseZero Coordinator Deployment: undone");

        super.destroy();
    }
}
