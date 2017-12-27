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

package com.arjuna.mwlabs.wsc.model.twophase.participants;

import com.arjuna.mwlabs.wsc.model.twophase.RegistrarImple;

import com.arjuna.mw.wscf.model.twophase.common.*;
import com.arjuna.mw.wscf.model.twophase.outcomes.*;
import com.arjuna.mw.wscf.model.twophase.participants.*;
import com.arjuna.mw.wscf.model.twophase.exceptions.*;
import com.arjuna.mw.wscf.model.twophase.vote.*;

import com.arjuna.mw.wscf.exceptions.*;

import com.arjuna.mw.wsas.exceptions.SystemException;
import com.arjuna.mw.wsas.exceptions.WrongStateException;
import com.arjuna.mw.wsas.exceptions.ProtocolViolationException;

/**
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: CleanupSynchronization.java,v 1.2 2003/02/19 16:00:20 nmcl Exp $
 * @since 1.0.
 */

public class CleanupSynchronization implements Synchronization
{

    public CleanupSynchronization (String cleanupId, RegistrarImple theRegistrar)
    {
	_cleanupId = cleanupId;
	_theRegistrar = theRegistrar;
    }

    public void beforeCompletion () throws SystemException
    {
	System.out.println("CleanupSynchronization.beforeCompletion");
    }

    public void afterCompletion (int status) throws SystemException
    {
	System.out.println("CleanupSynchronization.afterCompletion ( "+CoordinationResult.stringForm(status)+" )");

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
