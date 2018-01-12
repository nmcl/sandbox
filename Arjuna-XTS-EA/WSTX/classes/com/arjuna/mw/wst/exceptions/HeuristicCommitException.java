/*
 * Copyright (C) 2003,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: HeuristicCommitException.java,v 1.1 2003/02/03 11:01:25 nmcl Exp $
 */

package com.arjuna.mw.wst.exceptions;

/**
 * Thrown if a participant or transaction commit when the request was to
 * roll back. Can only happen for a participant if it has received a
 * previous prepare message.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: HeuristicCommitException.java,v 1.1 2003/02/03 11:01:25 nmcl Exp $
 * @since 1.0.
 */

public class HeuristicCommitException extends Exception
{

    public HeuristicCommitException ()
    {
	super();
    }

    public HeuristicCommitException (String s)
    {
	super(s);
    }
    
}


