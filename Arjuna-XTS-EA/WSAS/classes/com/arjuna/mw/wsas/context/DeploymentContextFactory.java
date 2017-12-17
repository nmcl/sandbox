/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: DeploymentContextFactory.java,v 1.2 2003/01/07 10:22:50 nmcl Exp $
 */

package com.arjuna.mw.wsas.context;

import com.arjuna.mw.wsas.common.Environment;

import com.arjuna.mwlabs.wsas.context.DeploymentContextImple;

/**
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: DeploymentContextFactory.java,v 1.2 2003/01/07 10:22:50 nmcl Exp $
 * @since 1.0.
 */

public class DeploymentContextFactory
{
    
    public static DeploymentContext deploymentContext ()
    {
	return _deployContext;
    }
    
    private static DeploymentContext _deployContext = null;

    static
    {
	String contextImple = System.getProperty(Environment.DEPLOYMENT_CONTEXT);

	try
	{
	    if (contextImple != null)
	    {
		Class c = Class.forName(contextImple);

		_deployContext = (DeploymentContext) c.newInstance();
	    }
	}
	catch (Exception ex)
	{
	    ex.printStackTrace();
	}
	
	if (_deployContext == null)
	    _deployContext = new DeploymentContextImple();
    }
    
}
