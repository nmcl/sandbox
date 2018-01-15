/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: PhaseZeroSynchronization.java,v 1.3 2003/02/11 15:12:23 nmcl Exp $
 */

package com.arjuna.mwlabs.wst.at.participants;

import com.arjuna.wst.PhaseZeroParticipant;

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
 * @version $Id: PhaseZeroSynchronization.java,v 1.3 2003/02/11 15:12:23 nmcl Exp $
 * @since 1.0.
 */

public class PhaseZeroSynchronization implements Synchronization
{

    public PhaseZeroSynchronization (PhaseZeroParticipant resource)
    {
	_resource = resource;
    }

    public void beforeCompletion () throws SystemException
    {
	if (_resource != null)
	{
	    try
	    {
		_resource.phaseZero();
	    }
	    //	    catch (com.arjuna.mw.wst.exceptions.SystemException ex)
	    catch (com.arjuna.wst.SystemException ex)
	    {
		throw new SystemException(ex.toString());
	    }
	}
	else
	    throw new SystemException("Null phase zero participant!");
    }

    public void afterCompletion (int status) throws SystemException
    {
	// do nothing
    }

    private PhaseZeroParticipant _resource;
    
}
