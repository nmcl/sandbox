/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: CleanupSynchronization.java,v 1.2 2003/02/19 16:13:04 nmcl Exp $
 */

package com.arjuna.mwlabs.wst.at.participants;

import com.arjuna.mwlabs.wst.at.RegistrarImple;

import com.arjuna.wsc.RegistrarMapper;

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
 * @version $Id: CleanupSynchronization.java,v 1.2 2003/02/19 16:13:04 nmcl Exp $
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
	// do nothing
    }

    public void afterCompletion (int status) throws SystemException
    {
	try
	{
	    _theRegistrar.disassociate(_cleanupId);
	}
	catch (Exception ex)
	{
	    ex.printStackTrace();
	    
	    throw new SystemException(ex.toString());
	}
    }

    private String         _cleanupId;
    private RegistrarImple _theRegistrar;
    
}
