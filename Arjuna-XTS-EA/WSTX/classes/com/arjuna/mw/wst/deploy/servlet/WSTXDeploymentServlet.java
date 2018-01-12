/*
 * Copyright (C) 2003,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: WSTXDeploymentServlet.java,v 1.2 2003/03/24 10:57:01 nmcl Exp $
 */

package com.arjuna.mw.wst.deploy.servlet;

import javax.servlet.ServletException;
import javax.servlet.ServletConfig;
import javax.servlet.http.HttpServlet;

import javax.naming.NamingException;

public class WSTXDeploymentServlet extends HttpServlet
{

    public void init (ServletConfig config) throws ServletException
    {
        super.init(config);

        try
        {
	    boolean rebind = false;
	
	    try
	    {
		com.arjuna.mwlabs.wst.util.Binder.bind("/wst.xml");
	    }
	    catch (NamingException ex)
	    {
		rebind = true;
	    }

	    if (rebind)
	    {
		com.arjuna.mwlabs.wst.util.Binder.rebind("/wst.xml");
	    }

	    com.arjuna.mw.wsas.utils.Configuration.initialise("/wstx.xml");

            log("WSTX Deployment: done");
        }
        catch (Exception exception)
        {
            log("WSTX Deployment: failed", exception);

            throw new ServletException("WSTX Servlet: init failed", exception);
        }
        catch (Error error)
        {
            log("WSTX Deployment: failed", error);

            throw new ServletException("WSTX Servlet: init failed", error);
        }
    }

    public void destroy()
    {
        log("WSTX Deployment: undone");

        super.destroy();
    }

}
