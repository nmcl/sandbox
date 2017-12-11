/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * ActivationDispatcherDeploymentServlet.java
 */

package com.arjuna.wsc.dispatcher.deploy.servlet;

import javax.servlet.ServletException;
import javax.servlet.ServletConfig;
import javax.servlet.http.HttpServlet;
import com.arjuna.wsc.impl.dispatcher.ActivationDispatcher;

public class ActivationDispatcherDeploymentServlet extends HttpServlet
{
    public void init(ServletConfig config)
        throws ServletException
    {
        super.init(config);

        try
        {
            String activationCoordinatorJNDIName = config.getInitParameter("ActivationCoordinatorJNDIName");
            String contextFactoryMapperJNDIName  = config.getInitParameter("ContextFactoryMapperJNDIName");

            _activationDispatcher = new ActivationDispatcher(activationCoordinatorJNDIName, contextFactoryMapperJNDIName);

            log("Activation Dispatcher Deployment: done, activation coordinator at \"" + activationCoordinatorJNDIName + "\", context factory mapper at \"" + contextFactoryMapperJNDIName + "\"");
        }
        catch (Exception exception)
        {
            log("Activation Dispatcher Deployment: failed", exception);

            throw new ServletException("Activation Dispatcher Servlet: init failed", exception);
        }
        catch (Error error)
        {
            log("Activation Dispatcher Deployment: failed", error);

            throw new ServletException("Activation Dispatcher Servlet: init failed", error);
        }
    }

    public void destroy()
    {
        log("Activation Dispatcher Deployment: undone");

        super.destroy();
    }

    private ActivationDispatcher _activationDispatcher = null;
}
