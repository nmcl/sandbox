/*
 * Copyright (C) 2003,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: SystemException.java,v 1.1 2003/02/03 11:01:26 nmcl Exp $
 */

package com.arjuna.mw.wst.exceptions;

/**
 * Thrown if the transaction is unknown.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: SystemException.java,v 1.1 2003/02/03 11:01:26 nmcl Exp $
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

