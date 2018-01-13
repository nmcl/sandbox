/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Vote.java,v 1.1 2002/11/25 11:00:51 nmcl Exp $
 */

package com.arjuna.mw.wstx.common;

import com.arjuna.mw.wscf.common.Qualifier;

/**
 * All votes from prepare are instances of this interface.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: Vote.java,v 1.1 2002/11/25 11:00:51 nmcl Exp $
 * @since 1.0.
 */

public abstract class Vote
{

    /**
     * @return any qualifiers that may be associated with the vote. May be
     * null.
     */

    public Qualifier[] getQualifiers ()
    {
	return _qualifiers;
    }

    public abstract String toString ();

    /**
     * Currently only check actual vote type and not the qualifiers.
     */

    public abstract boolean equals (Object o);
    
    protected Vote ()
    {
	this(null);
    }
    
    protected Vote (Qualifier[] quals)
    {
	_qualifiers = quals;
    }

    /**
     * @return a printable version of the vote.
     */

    protected Qualifier[] _qualifiers;

}
