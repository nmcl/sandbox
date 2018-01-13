/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: HeuristicCommitException.java,v 1.1 2002/11/25 11:00:52 nmcl Exp $
 */

package com.arjuna.mw.wstx.exceptions;

import com.arjuna.mw.wsas.exceptions.WSASException;

/**
 * Thrown if a participant or transaction commit when the request was to
 * roll back. Can only happen for a participant if it has received a
 * previous prepare message.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: HeuristicCommitException.java,v 1.1 2002/11/25 11:00:52 nmcl Exp $
 * @since 1.0.
 */

public class HeuristicCommitException extends WSASException
{

    public HeuristicCommitException ()
    {
	super();
    }

    public HeuristicCommitException (String s)
    {
	super(s);
    }

    public HeuristicCommitException (String s, int errorcode)
    {
	super(s, errorcode);
    }
    
}


