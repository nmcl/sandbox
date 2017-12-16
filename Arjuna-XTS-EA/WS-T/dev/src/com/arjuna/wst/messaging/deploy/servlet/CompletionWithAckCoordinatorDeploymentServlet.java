/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * CompletionWithAckCoordinatorDeploymentServlet.java
 */

package com.arjuna.wst.messaging.deploy.servlet;

import javax.naming.InitialContext;
import javax.servlet.ServletException;
import javax.servlet.ServletConfig;
import javax.servlet.http.HttpServlet;
import com.arjuna.wst.messaging.CompletionWithAckCoordinator;
import com.arjuna.wst.impl.messaging.saaj.CompletionWithAckCoordinatorImpl;

public class CompletionWithAckCoordinatorDeploymentServlet extends HttpServlet
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

            CompletionWithAckCoordinator completionWithAckCoordinator = new CompletionWithAckCoordinatorImpl(serviceURI);

            initialContext.bind(jndiName, completionWithAckCoordinator);

            log("CompletionWithAck Coordinator Deployment: done, completion with ack coordinator at \"" + serviceURI + "\", jndi \"" + jndiName + "\"");
        }
        catch (Exception exception)
        {
            log("CompletionWithAck Coordinator Deployment: failed", exception);

            throw new ServletException("CompletionWithAck Coordinator Deployment Servlet: init failed", exception);
        }
        catch (Error error)
        {
            log("CompletionWithAck Coordinator Deployment: failed", error);

            throw new ServletException("CompletionWithAck Coordinator Deployment Servlet: init failed", error);
        }
    }

    public void destroy()
    {
        log("CompletionWithAck Coordinator Deployment: undone");

        super.destroy();
    }
}
