/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: VoteCancel.java,v 1.1 2003/01/07 10:33:43 nmcl Exp $
 */

package com.arjuna.mw.wscf.model.as.coordinator.twophase.outcomes;

import com.arjuna.mw.wscf.common.Qualifier;

import com.arjuna.mw.wsas.activity.Outcome;

import com.arjuna.mw.wsas.completionstatus.CompletionStatus;
import com.arjuna.mw.wsas.completionstatus.Failure;

import com.arjuna.mw.wsas.exceptions.SystemException;

import java.util.Arrays;

/**
 * The inferior votes that is has cancelled. The coordinator service
 * may inform the inferior of the final decision (hopefully to cancel
 * as well), but it need not.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: VoteCancel.java,v 1.1 2003/01/07 10:33:43 nmcl Exp $
 */

public class VoteCancel implements Vote
{

    public VoteCancel ()
    {
	_quals = null;
    }
    
    public VoteCancel (Qualifier[] quals)
    {
	_quals = quals;
    }

    public boolean equals (Object o)
    {
	if (o == null)
	    return false;

	if (o instanceof VoteCancel)
	    return true;
	else
	    return false;
    }

    /**
     * @return a printable version of the vote.
     */

    public String toString ()
    {
	return "org.w3c.wscf.twophase.outcomes.VoteCancel";
    }

    public CompletionStatus completedStatus () throws SystemException
    {
	return Failure.instance();
    }
    
    public String name () throws SystemException
    {
	return toString();
    }

    public Object data () throws SystemException
    {
	return Arrays.asList(_quals);
    }
    
    private Qualifier[] _quals;
    
}
