/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: BegunFailedException.java,v 1.1 2002/11/25 10:56:34 nmcl Exp $
 */

package com.arjuna.mw.wscf.exceptions;

import com.arjuna.mw.wsas.exceptions.SystemException;

/**
 * The coordination protocol failed to start.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: BegunFailedException.java,v 1.1 2002/11/25 10:56:34 nmcl Exp $
 * @since 1.0.
 */

public class BegunFailedException extends SystemException
{

    public BegunFailedException ()
    {
	super();
    }

    public BegunFailedException (String s)
    {
	super(s);
    }

    public BegunFailedException (String s, int errorcode)
    {
	super(s, errorcode);
    }
    
}


