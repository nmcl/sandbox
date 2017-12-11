/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * ActivationRequesterDeploymentServlet.java
 */

package com.arjuna.wsc.messaging.deploy.servlet;

import javax.naming.InitialContext;
import javax.servlet.ServletException;
import javax.servlet.ServletConfig;
import javax.servlet.http.HttpServlet;
import com.arjuna.wsc.messaging.ActivationRequester;
import com.arjuna.wsc.impl.messaging.saaj.ActivationRequesterImpl;

public class ActivationRequesterDeploymentServlet extends HttpServlet
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

            ActivationRequester activationRequester = new ActivationRequesterImpl(serviceURI);

            initialContext.bind(jndiName, activationRequester);

            log("Activation Requester Deployment: done, activation requester at \"" + serviceURI + "\", jndi \"" + jndiName + "\"");
        }
        catch (Exception exception)
        {
            log("Activation Requester Deployment: failed", exception);

            throw new ServletException("Activation Requester Deployment Servlet: init failed", exception);
        }
        catch (Error error)
        {
            log("Activation Requester Deployment: failed", error);

            throw new ServletException("Activation Requester Deployment Servlet: init failed", error);
        }
    }

    public void destroy()
    {
        log("Activation Requester Deployment: undone");

        super.destroy();
    }
}
