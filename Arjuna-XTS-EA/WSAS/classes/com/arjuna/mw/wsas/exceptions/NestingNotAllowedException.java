/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: NestingNotAllowedException.java,v 1.1 2002/11/25 10:51:43 nmcl Exp $
 */

package com.arjuna.mw.wsas.exceptions;

/**
 * Not all HLSs allow nesting of activities to occur. This
 * exception is thrown if an attempt is made to create a new activity
 * when a activity is already associated with the invoking thread and
 * the activity implementation does not support nesting.
 * 
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: NestingNotAllowedException.java,v 1.1 2002/11/25 10:51:43 nmcl Exp $
 * @since 1.0.
 */

public class NestingNotAllowedException extends HLSException
{

    public NestingNotAllowedException ()
    {
	super();
    }

    public NestingNotAllowedException (String s)
    {
	super(s);
    }

    public NestingNotAllowedException (String s, int errorcode)
    {
	super(s, errorcode);
    }
    
}


