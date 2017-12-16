/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * TwoPCCoordinatorDeploymentServlet.java
 */

package com.arjuna.wst.messaging.deploy.servlet;

import javax.naming.InitialContext;
import javax.servlet.ServletException;
import javax.servlet.ServletConfig;
import javax.servlet.http.HttpServlet;
import com.arjuna.wst.messaging.TwoPCCoordinator;
import com.arjuna.wst.impl.messaging.saaj.TwoPCCoordinatorImpl;

public class TwoPCCoordinatorDeploymentServlet extends HttpServlet
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

            TwoPCCoordinator twoPCCoordinator = new TwoPCCoordinatorImpl(serviceURI);

            initialContext.bind(jndiName, twoPCCoordinator);

            log("TwoPC Coordinator Deployment: done, two pc coordinator at \"" + serviceURI + "\", jndi \"" + jndiName + "\"");
        }
        catch (Exception exception)
        {
            log("TwoPC Coordinator Deployment: failed", exception);

            throw new ServletException("TwoPC Coordinator Deployment Servlet: init failed", exception);
        }
        catch (Error error)
        {
            log("TwoPC Coordinator Deployment: failed", error);

            throw new ServletException("TwoPC Coordinator Deployment Servlet: init failed", error);
        }
    }

    public void destroy()
    {
        log("TwoPC Coordinator Deployment: undone");

        super.destroy();
    }
}
