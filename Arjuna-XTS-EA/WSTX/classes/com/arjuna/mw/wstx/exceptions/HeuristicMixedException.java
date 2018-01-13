/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: HeuristicMixedException.java,v 1.1 2002/11/25 11:00:52 nmcl Exp $
 */

package com.arjuna.mw.wstx.exceptions;

import com.arjuna.mw.wsas.exceptions.WSASException;

/**
 * Thrown if some of the participants commit while others rollback.
 * Can only happen for a participant if it has received a
 * previous prepare message.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: HeuristicMixedException.java,v 1.1 2002/11/25 11:00:52 nmcl Exp $
 * @since 1.0.
 */

public class HeuristicMixedException extends WSASException
{

    public HeuristicMixedException ()
    {
	super();
    }

    public HeuristicMixedException (String s)
    {
	super(s);
    }

    public HeuristicMixedException (String s, int errorcode)
    {
	super(s, errorcode);
    }
    
}


