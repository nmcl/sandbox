/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: OutcomeNotificationSynchronization.java,v 1.4 2003/03/04 12:59:29 nmcl Exp $
 */

package com.arjuna.mwlabs.wst.at.participants;

import com.arjuna.wst.OutcomeNotificationParticipant;

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
 * @version $Id: OutcomeNotificationSynchronization.java,v 1.4 2003/03/04 12:59:29 nmcl Exp $
 * @since 1.0.
 */

public class OutcomeNotificationSynchronization implements Synchronization
{

    public OutcomeNotificationSynchronization (OutcomeNotificationParticipant resource)
    {
	_resource = resource;
    }

    public void beforeCompletion () throws SystemException
    {
	// do nothing
    }

    public void afterCompletion (int status) throws SystemException
    {
	if (_resource != null)
	{
	    try
	    {
		if (status == CoordinationResult.CONFIRMED)
		    _resource.committed();
		else
		    _resource.aborted();
	    }
	    //	    catch (com.arjuna.mw.wst.exceptions.SystemException ex)
	    catch (com.arjuna.wst.SystemException ex)
	    {
		throw new SystemException(ex.toString());
	    }
	}
	else
	    throw new SystemException("Null outcome notification participant!");
    }

    private OutcomeNotificationParticipant _resource;
    
}
