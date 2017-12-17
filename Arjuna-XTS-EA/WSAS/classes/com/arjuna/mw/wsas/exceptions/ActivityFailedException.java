/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ActivityFailedException.java,v 1.1 2002/11/25 10:51:42 nmcl Exp $
 */

package com.arjuna.mw.wsas.exceptions;

/**
 * Thrown if the activity has failed.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: ActivityFailedException.java,v 1.1 2002/11/25 10:51:42 nmcl Exp $
 * @since 1.0.
 */

public class ActivityFailedException extends WSASException
{

    public ActivityFailedException ()
    {
	super();
    }

    public ActivityFailedException (String s)
    {
	super(s);
    }

    public ActivityFailedException (String s, int errorcode)
    {
	super(s, errorcode);
    }
    
}


