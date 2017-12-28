/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: TwoPhaseSynchronization.java,v 1.2 2003/02/24 13:21:33 nmcl Exp $
 */

package com.arjuna.mwlabs.wsc.model.twophase.participants;

import com.arjuna.wsc.messaging.PortReference;

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
 * @version $Id: TwoPhaseSynchronization.java,v 1.2 2003/02/24 13:21:33 nmcl Exp $
 * @since 1.0.
 */

public class TwoPhaseSynchronization implements Synchronization
{

    public TwoPhaseSynchronization (PortReference address)
    {
	_address = address;
    }

    public void beforeCompletion () throws SystemException
    {
	System.out.println("TwoPhaseSynchronization.beforeCompletion");
    }

    public void afterCompletion (int status) throws SystemException
    {
	System.out.println("TwoPhaseSynchronization.afterCompletion ( "+CoordinationResult.stringForm(status)+" )");
    }

    private PortReference _address;
    
}
