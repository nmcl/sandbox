/*
 * Copyright (C) 2003,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: WSCFDeploymentServlet.java,v 1.3 2003/04/04 15:00:06 nmcl Exp $
 */

package com.arjuna.mw.wsc.deploy.servlet;

import com.arjuna.wsc.ContextFactoryMapper;

import javax.servlet.ServletException;
import javax.servlet.ServletConfig;
import javax.servlet.http.HttpServlet;

import javax.naming.InitialContext;

public class WSCFDeploymentServlet extends HttpServlet
{

    public void init (ServletConfig config) throws ServletException
    {
        super.init(config);

        try
        {
            String jndiName   = config.getInitParameter("JNDIName");

	    InitialContext initialContext = new InitialContext();
	    
	    ContextFactoryMapper wscfImpl = (ContextFactoryMapper)initialContext.lookup(jndiName);

            String serviceURI = wscfImpl.getServiceURI();

	    wscfImpl.setSubordinateContextFactoryMapper(new com.arjuna.mwlabs.wsc.ContextFactoryMapperImple(serviceURI));

	    com.arjuna.mw.wsas.utils.Configuration.initialise("/wscf.xml");
	    
            log("WSCF Deployment: done");
        }
        catch (Exception exception)
        {
            log("WSCF Deployment: failed", exception);

            throw new ServletException("WSCF Servlet: init failed", exception);
        }
        catch (Error error)
        {
            log("WSCF Deployment: failed", error);

            throw new ServletException("WSCF Servlet: init failed", error);
        }
    }

    public void destroy()
    {
        log("WSCF Deployment: undone");

        super.destroy();
    }

}
