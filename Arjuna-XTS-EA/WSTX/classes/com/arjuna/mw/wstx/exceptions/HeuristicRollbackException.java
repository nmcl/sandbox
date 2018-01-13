/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: HeuristicRollbackException.java,v 1.1 2002/11/25 11:00:52 nmcl Exp $
 */

package com.arjuna.mw.wstx.exceptions;

import com.arjuna.mw.wsas.exceptions.WSASException;

/**
 * Thrown if the participants rolled back when the request was to commit.
 * Can only happen for a participant if it has received a
 * previous prepare message.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: HeuristicRollbackException.java,v 1.1 2002/11/25 11:00:52 nmcl Exp $
 * @since 1.0.
 */

public class HeuristicRollbackException extends WSASException
{

    public HeuristicRollbackException ()
    {
	super();
    }

    public HeuristicRollbackException (String s)
    {
	super(s);
    }

    public HeuristicRollbackException (String s, int errorcode)
    {
	super(s, errorcode);
    }
    
}


