/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: VoteCommit.java,v 1.1 2002/11/25 11:00:51 nmcl Exp $
 */

package com.arjuna.mw.wstx.common;

import com.arjuna.mw.wscf.common.Qualifier;

/**
 * The inferior votes to that it can confirm. It will typically
 * not have confirmed at this stage but will wait for the transaction
 * outcome. Failure to do so may result in heuristics (contradictions).
 *
 * @author Mark Little (mark_little@hp.com)
 * @version $Id: VoteCommit.java,v 1.1 2002/11/25 11:00:51 nmcl Exp $
 * @since XTS 1.0.
 */

public class VoteCommit extends Vote
{

    public VoteCommit ()
    {
	super();
    }
    
    public VoteCommit (Qualifier[] quals)
    {
	super(quals);
    }
    
    public boolean equals (Object o)
    {
	if (o == null)
	    return false;

	if (o instanceof VoteCommit)
	    return true;
	else
	    return false;
    }
    
    /**
     * @return a printable version of the vote.
     */

    public String toString ()
    {
	return "VoteCommit";
    }
    
}
