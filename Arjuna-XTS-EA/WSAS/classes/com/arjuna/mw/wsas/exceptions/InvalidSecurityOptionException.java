/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: InvalidSecurityOptionException.java,v 1.1 2002/11/25 10:51:43 nmcl Exp $
 */

package com.arjuna.mw.wsas.exceptions;

/**
 * The invoker does not have sufficient access rights to attempt the
 * operation. For example, obtaining all of the coordinator implementations
 * that are provided by the underlying implementation.
 * 
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: InvalidSecurityOptionException.java,v 1.1 2002/11/25 10:51:43 nmcl Exp $
 * @since 1.0.
 */

public class InvalidSecurityOptionException extends WSASException
{

    public InvalidSecurityOptionException ()
    {
	super();
    }

    public InvalidSecurityOptionException (String s)
    {
	super(s);
    }

    public InvalidSecurityOptionException (String s, int errorcode)
    {
	super(s, errorcode);
    }
    
}
