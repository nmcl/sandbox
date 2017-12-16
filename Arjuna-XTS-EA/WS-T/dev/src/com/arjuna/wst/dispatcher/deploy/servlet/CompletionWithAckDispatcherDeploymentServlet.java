/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * CompletionWithAckDispatcherDeploymentServlet.java
 */

package com.arjuna.wst.dispatcher.deploy.servlet;

import javax.naming.InitialContext;
import javax.servlet.ServletException;
import javax.servlet.ServletConfig;
import javax.servlet.http.HttpServlet;
import com.arjuna.wst.dispatcher.CompletionWithAckDispatcher;
import com.arjuna.wst.messaging.CompletionWithAckCoordinator;

public class CompletionWithAckDispatcherDeploymentServlet extends HttpServlet
{
    public void init(ServletConfig config)
        throws ServletException
    {
        super.init(config);

        try
        {
            InitialContext initialContext = new InitialContext();

            String jndiName                             = config.getInitParameter("JNDIName");
            String completionWithAckCoordinatorJNDIName = config.getInitParameter("CompletionWithAckCoordinatorJNDIName");

            CompletionWithAckCoordinator completionWithAckCoordinator = (CompletionWithAckCoordinator) initialContext.lookup(completionWithAckCoordinatorJNDIName);

            CompletionWithAckDispatcher completionWithAckDispatcher = new CompletionWithAckDispatcher(completionWithAckCoordinator);

            initialContext.bind(jndiName, completionWithAckDispatcher);

            log("Completion Dispatcher With Ack Deployment: done, completion with ack coordinator at \"" + completionWithAckCoordinatorJNDIName + "\", jndi \"" + jndiName + "\"");
        }
        catch (Exception exception)
        {
            log("Completion With Ack Dispatcher Deployment: failed", exception);

            throw new ServletException("Completion With Ack Dispatcher Deployment Servlet: init failed", exception);
        }
        catch (Error error)
        {
            log("Completion With Ack Dispatcher Deployment: failed", error);

            throw new ServletException("Completion With Ack Dispatcher Deployment Servlet: init failed", error);
        }
    }

    public void destroy()
    {
        log("Completion With Ack Dispatcher Deployment: undone");

        super.destroy();
    }
}
