/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: InvalidHLSException.java,v 1.1 2002/11/25 10:51:43 nmcl Exp $
 */

package com.arjuna.mw.wsas.exceptions;

/**
 * Thrown if an attempt is made to unregister an unknown HLS from the activity.
 * 
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: InvalidHLSException.java,v 1.1 2002/11/25 10:51:43 nmcl Exp $
 * @since 1.0.
 */

public class InvalidHLSException extends WSASException
{

    public InvalidHLSException ()
    {
	super();
    }

    public InvalidHLSException (String s)
    {
	super(s);
    }

    public InvalidHLSException (String s, int errorcode)
    {
	super(s, errorcode);
    }
    
}


