/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: InvalidSynchronizationException.java,v 1.1 2002/11/25 11:00:52 nmcl Exp $
 */

package com.arjuna.mw.wstx.exceptions;

import com.arjuna.mw.wsas.exceptions.WSASException;

/**
 * Thrown if a resign is attempted on a synchronization that the transaction
 * did not know about.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: InvalidSynchronizationException.java,v 1.1 2002/11/25 11:00:52 nmcl Exp $
 * @since 1.0.
 */

public class InvalidSynchronizationException extends WSASException
{
    
    public InvalidSynchronizationException ()
    {
	super();
    }

    public InvalidSynchronizationException (String s)
    {
	super(s);
    }

    public InvalidSynchronizationException (String s, int errorcode)
    {
	super(s, errorcode);
    }
 
}

