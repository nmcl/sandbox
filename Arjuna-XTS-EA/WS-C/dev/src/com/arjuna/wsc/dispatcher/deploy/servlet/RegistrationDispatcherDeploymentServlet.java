/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * RegistrationDispatcherDeploymentServlet.java
 */

package com.arjuna.wsc.dispatcher.deploy.servlet;

import javax.servlet.ServletException;
import javax.servlet.ServletConfig;
import javax.servlet.http.HttpServlet;
import com.arjuna.wsc.impl.dispatcher.RegistrationDispatcher;

public class RegistrationDispatcherDeploymentServlet extends HttpServlet
{
    public void init(ServletConfig config)
        throws ServletException
    {
        super.init(config);

        try
        {
            String registrationCoordinatorJNDIName = config.getInitParameter("RegistrationCoordinatorJNDIName");
            String registrarMapperJNDIName         = config.getInitParameter("RegistrarMapperJNDIName");

            _registrationDispatcher = new RegistrationDispatcher(registrationCoordinatorJNDIName, registrarMapperJNDIName);

            log("Registration Dispatcher Deployment: done, registration coordinator at \"" + registrationCoordinatorJNDIName + "\", registrar mapper at \"" + registrarMapperJNDIName + "\"");
        }
        catch (Exception exception)
        {
            log("Registration Dispatcher Deployment: failed", exception);

            throw new ServletException("Registration Dispatcher Servlet: init failed", exception);
        }
        catch (Error error)
        {
            log("Registration Dispatcher Deployment: failed", error);

            throw new ServletException("Registration Dispatcher Servlet: init failed", error);
        }
    }

    public void destroy()
    {
        log("Registration Dispatcher Deployment: undone");

        super.destroy();
    }

    private RegistrationDispatcher _registrationDispatcher = null;
}
