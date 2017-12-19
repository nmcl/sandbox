/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: DeploymentContextImple.java,v 1.2 2003/01/07 10:22:50 nmcl Exp $
 */

package com.arjuna.mwlabs.wsas.context;

import com.arjuna.mw.wsas.context.*;

import com.arjuna.mwlabs.wsas.context.soap.SOAPContextImple;

/**
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: DeploymentContextImple.java,v 1.2 2003/01/07 10:22:50 nmcl Exp $
 * @since 1.0.
 */

public class DeploymentContextImple implements DeploymentContext
{

    public DeploymentContextImple ()
    {
    }
    
    public Context context ()
    {
	return new SOAPContextImple();
    }

}
