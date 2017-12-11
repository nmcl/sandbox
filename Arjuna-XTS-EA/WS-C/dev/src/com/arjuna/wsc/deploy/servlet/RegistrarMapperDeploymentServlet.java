/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * RegistrarMapperDeploymentServlet.java
 */

package com.arjuna.wsc.deploy.servlet;

import javax.servlet.ServletException;
import javax.servlet.ServletConfig;
import javax.servlet.http.HttpServlet;
import com.arjuna.wsc.impl.messaging.RegistrarMapperImpl;

public class RegistrarMapperDeploymentServlet extends HttpServlet
{
    public void init(ServletConfig config)
        throws ServletException
    {
        super.init(config);

        try
        {
            String jndiName   = config.getInitParameter("JNDIName");
            String serviceURI = config.getInitParameter("ServiceURI");

            _registrarMapper = new RegistrarMapperImpl(jndiName, serviceURI);

            log("Registrar Mapper Deployment: done, service at \"" + serviceURI + "\", registration mapper at \"" + jndiName + "\"");
        }
        catch (Exception exception)
        {
            log("Registrar Mapper Deployment: failed", exception);

            throw new ServletException("Registrar Mapper Servlet: init failed", exception);
        }
        catch (Error error)
        {
            log("Registrar Mapper Deployment: failed", error);

            throw new ServletException("Registrar Mapper Servlet: init failed", error);
        }
    }

    public void destroy()
    {
        log("Registrar Mapper Deployment: undone");

        super.destroy();
    }

    private RegistrarMapperImpl _registrarMapper = null;
}
