/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: VoteReadOnly.java,v 1.1 2002/11/25 11:00:51 nmcl Exp $
 */

package com.arjuna.mw.wstx.common;

import com.arjuna.mw.wscf.common.Qualifier;

/**
 * The inferior has not done any work that affects the transaction.
 * It is effectively resigning.
 *
 * WARNING: this should be used with care, since the participant will
 * not then find out the actual transaction outcome.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: VoteReadOnly.java,v 1.1 2002/11/25 11:00:51 nmcl Exp $
 * @since 1.0.
 */

public class VoteReadOnly extends Vote
{

    public VoteReadOnly ()
    {
	super();
    }
    
    public VoteReadOnly (Qualifier[] quals)
    {
	super(quals);
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

    public String toString ()
    {
	return "VoteReadOnly";
    }
    
}
