/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * RegistrationRequesterDeploymentServlet.java
 */

package com.arjuna.wsc.messaging.deploy.servlet;

import javax.naming.InitialContext;
import javax.servlet.ServletException;
import javax.servlet.ServletConfig;
import javax.servlet.http.HttpServlet;
import com.arjuna.wsc.messaging.RegistrationRequester;
import com.arjuna.wsc.impl.messaging.saaj.RegistrationRequesterImpl;

public class RegistrationRequesterDeploymentServlet extends HttpServlet
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

            RegistrationRequester registrationRequester = new RegistrationRequesterImpl(serviceURI);

            initialContext.bind(jndiName, registrationRequester);

            log("Registration Requester Deployment: done, registration requester at \"" + serviceURI + "\", jndi \"" + jndiName + "\"");
        }
        catch (Exception exception)
        {
            log("Registration Requester Deployment: failed", exception);

            throw new ServletException("Registration Requester Deployment Servlet: init failed", exception);
        }
        catch (Error error)
        {
            log("Registration Requester Deployment: failed", error);

            throw new ServletException("Registration Requester Deployment Servlet: init failed", error);
        }
    }

    public void destroy()
    {
        log("Registration Requester Deployment: undone");

        super.destroy();
    }
}
