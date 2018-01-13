/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: HeuristicHazardException.java,v 1.1 2002/11/25 11:00:52 nmcl Exp $
 */

package com.arjuna.mw.wstx.exceptions;

import com.arjuna.mw.wsas.exceptions.WSASException;

/**
 * Thrown if the outcome of at least one participant is unknown.
 * Can only happen for a participant if it has received a
 * previous prepare message.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: HeuristicHazardException.java,v 1.1 2002/11/25 11:00:52 nmcl Exp $
 * @since 1.0.
 */

public class HeuristicHazardException extends WSASException
{

    public HeuristicHazardException ()
    {
	super();
    }

    public HeuristicHazardException (String s)
    {
	super(s);
    }

    public HeuristicHazardException (String s, int errorcode)
    {
	super(s, errorcode);
    }
    
}


