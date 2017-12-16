/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * CompletionDispatcherDeploymentServlet.java
 */

package com.arjuna.wst.dispatcher.deploy.servlet;

import javax.naming.InitialContext;
import javax.servlet.ServletException;
import javax.servlet.ServletConfig;
import javax.servlet.http.HttpServlet;
import com.arjuna.wst.dispatcher.CompletionDispatcher;
import com.arjuna.wst.messaging.CompletionCoordinator;

public class CompletionDispatcherDeploymentServlet extends HttpServlet
{
    public void init(ServletConfig config)
        throws ServletException
    {
        super.init(config);

        try
        {
            InitialContext initialContext = new InitialContext();

            String jndiName                      = config.getInitParameter("JNDIName");
            String completionCoordinatorJNDIName = config.getInitParameter("CompletionCoordinatorJNDIName");

            CompletionCoordinator completionCoordinator = (CompletionCoordinator) initialContext.lookup(completionCoordinatorJNDIName);

            CompletionDispatcher completionDispatcher = new CompletionDispatcher(completionCoordinator);

            initialContext.bind(jndiName, completionDispatcher);

            log("Completion Dispatcher Deployment: done, completion coordinator at \"" + completionCoordinator + "\", jndi \"" + jndiName + "\"");
        }
        catch (Exception exception)
        {
            log("Completion Dispatcher Deployment: failed", exception);

            throw new ServletException("Completion Dispatcher Deployment Servlet: init failed", exception);
        }
        catch (Error error)
        {
            log("Completion Dispatcher Deployment: failed", error);

            throw new ServletException("Completion Dispatcher Deployment Servlet: init failed", error);
        }
    }

    public void destroy()
    {
        log("Completion Dispatcher Deployment: undone");

        super.destroy();
    }
}
