/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: DeploymentContextImple.java,v 1.1 2003/02/03 11:01:28 nmcl Exp $
 */

package com.arjuna.mwlabs.wst.at.context;

import com.arjuna.mw.wsas.context.*;

/**
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: DeploymentContextImple.java,v 1.1 2003/02/03 11:01:28 nmcl Exp $
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
