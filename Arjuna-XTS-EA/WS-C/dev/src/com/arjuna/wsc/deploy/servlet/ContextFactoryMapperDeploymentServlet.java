/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * ContextFactoryMapperDeploymentServlet.java
 */

package com.arjuna.wsc.deploy.servlet;

import javax.servlet.ServletException;
import javax.servlet.ServletConfig;
import javax.servlet.http.HttpServlet;
import com.arjuna.wsc.impl.messaging.ContextFactoryMapperImpl;

public class ContextFactoryMapperDeploymentServlet extends HttpServlet
{
    public void init(ServletConfig config)
        throws ServletException
    {
        super.init(config);

        try
        {
            String jndiName   = config.getInitParameter("JNDIName");
            String serviceURI = config.getInitParameter("ServiceURI");

            _contextFactoryMapper = new ContextFactoryMapperImpl(jndiName, serviceURI);

            log("Context Factory Mapper Deployment: done, service at \"" + serviceURI + "\", context factory mapper at \"" + jndiName + "\"");
        }
        catch (Exception exception)
        {
            log("Context Factory Mapper Deployment: failed", exception);

            throw new ServletException("Context Factory Mapper Servlet: init failed", exception);
        }
        catch (Error error)
        {
            log("Context Factory Mapper Deployment: failed", error);

            throw new ServletException("Context Factory Mapper Servlet: init failed", error);
        }
    }

    public void destroy()
    {
        log("Context Factory Mapper Deployment: undone");

        super.destroy();
    }

    private ContextFactoryMapperImpl _contextFactoryMapper = null;
}
