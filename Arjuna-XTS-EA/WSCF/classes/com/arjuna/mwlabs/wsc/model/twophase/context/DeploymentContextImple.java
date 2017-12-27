/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: DeploymentContextImple.java,v 1.1 2003/01/07 10:33:49 nmcl Exp $
 */

package com.arjuna.mwlabs.wsc.model.twophase.context;

import com.arjuna.mw.wsas.context.*;

/**
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: DeploymentContextImple.java,v 1.1 2003/01/07 10:33:49 nmcl Exp $
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
