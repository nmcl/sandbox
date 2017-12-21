/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: VoteReadOnly.java,v 1.1 2003/01/07 10:33:43 nmcl Exp $
 */

package com.arjuna.mw.wscf.model.as.coordinator.twophase.outcomes;

import com.arjuna.mw.wscf.common.Qualifier;

import com.arjuna.mw.wsas.activity.Outcome;

import com.arjuna.mw.wsas.completionstatus.CompletionStatus;
import com.arjuna.mw.wsas.completionstatus.Success;

import com.arjuna.mw.wsas.exceptions.SystemException;

import java.util.Arrays;

/**
 * The inferior votes that it has done no work that requires to be involved
 * any further in the two-phase protocol. For example, it has not updated
 * any data. This can then be used by the coordinator to optimise the
 * subsequent phase of the protocol (if any).
 *
 * WARNING: this should be used with care.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: VoteReadOnly.java,v 1.1 2003/01/07 10:33:43 nmcl Exp $
 */

public class VoteReadOnly implements Vote
{

    public VoteReadOnly ()
    {
	_quals = null;
    }
    
    public VoteReadOnly (Qualifier[] quals)
    {
	_quals = quals;
    }

    public boolean equals (Object o)
    {
	if (o == null)
	    return false;

	if (o instanceof VoteReadOnly)
	    return true;
	else
	    return false;
    }

    /**
     * @return a printable version of the vote.
     */

    public String toString ()
    {
	return "org.w3c.wscf.twophase.outcomes.VoteReadOnly";
    }

    public CompletionStatus completedStatus () throws SystemException
    {
	return Success.instance();
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
