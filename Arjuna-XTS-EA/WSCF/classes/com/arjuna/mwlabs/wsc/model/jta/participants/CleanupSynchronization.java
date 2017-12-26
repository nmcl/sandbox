/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: CleanupSynchronization.java,v 1.2 2003/02/19 16:00:20 nmcl Exp $
 */

package com.arjuna.mwlabs.wsc.model.jta.participants;

import com.arjuna.mwlabs.wsc.model.jta.RegistrarImple;

import javax.transaction.xa.*;

/**
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: CleanupSynchronization.java,v 1.2 2003/02/19 16:00:20 nmcl Exp $
 * @since 1.0.
 */

public class CleanupSynchronization implements javax.transaction.Synchronization
{

    public CleanupSynchronization (String cleanupId, RegistrarImple theRegistrar)
    {
	_cleanupId = cleanupId;
	_theRegistrar = theRegistrar;
    }

    public void beforeCompletion ()
    {
	System.out.println("CleanupSynchronization.beforeCompletion");
    }

    public void afterCompletion (int status)
    {
	System.out.println("CleanupSynchronization.afterCompletion ( "+status+" )");

	try
	{
	    _theRegistrar.disassociate(_cleanupId);
	}
	catch (Exception ex)
	{
	    ex.printStackTrace();
	}
    }

    private String         _cleanupId;
    private RegistrarImple _theRegistrar;
    
}
