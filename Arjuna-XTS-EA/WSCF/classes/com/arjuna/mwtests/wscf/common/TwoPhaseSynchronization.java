/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: TwoPhaseSynchronization.java,v 1.1 2003/01/07 10:37:17 nmcl Exp $
 */

package com.arjuna.mwtests.wscf.common;

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
 * @version $Id: TwoPhaseSynchronization.java,v 1.1 2003/01/07 10:37:17 nmcl Exp $
 * @since 1.0.
 */

public class TwoPhaseSynchronization implements Synchronization
{

    public TwoPhaseSynchronization ()
    {
    }

    public void beforeCompletion () throws SystemException
    {
	System.out.println("TwoPhaseSynchronization.beforeCompletion");
    }

    public void afterCompletion (int status) throws SystemException
    {
	System.out.println("TwoPhaseSynchronization.afterCompletion ( "+CoordinationResult.stringForm(status)+" )");
    }

}
