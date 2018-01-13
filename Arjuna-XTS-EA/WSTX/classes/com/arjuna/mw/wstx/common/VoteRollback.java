/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: VoteRollback.java,v 1.1 2002/11/25 11:00:51 nmcl Exp $
 */

package com.arjuna.mw.wstx.common;

import com.arjuna.mw.wscf.common.Qualifier;

/**
 * The participant votes that is has rolled back. The transaction service
 * may inform the participant of the final decision (hopefully to rollback
 * as well), but it need not.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: VoteRollback.java,v 1.1 2002/11/25 11:00:51 nmcl Exp $ 
 * @since 1.0.
 */

public class VoteRollback extends Vote
{

    public VoteRollback ()
    {
	super();
    }
    
    public VoteRollback (Qualifier[] quals)
    {
	super(quals);
    }

    public boolean equals (Object o)
    {
	if (o == null)
	    return false;

	if (o instanceof VoteRollback)
	    return true;
	else
	    return false;
    }

    /**
     * @return a printable version of the vote.
     */

    public String toString ()
    {
	return "VoteRollback";
    }

}
