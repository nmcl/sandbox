/*
 * Copyright (C) 2003,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: SystemException.java,v 1.2 2003/04/11 14:47:29 nmcl Exp $
 */

package com.arjuna.wst;

/**
 * Thrown if a general exception is encountered (one not supported by
 * any other exception.)
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: SystemException.java,v 1.2 2003/04/11 14:47:29 nmcl Exp $
 * @since 1.0.
 */

public class SystemException extends Exception
{
    
    public SystemException ()
    {
	super();
    }

    public SystemException (String s)
    {
	super(s);
    }
 
}

