/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: NoTransactionException.java,v 1.1 2002/11/25 11:00:52 nmcl Exp $
 */

package com.arjuna.mw.wstx.exceptions;

import com.arjuna.mw.wsas.exceptions.WSASException;

/**
 * Thrown if no transaction is associated with the current thread.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: NoTransactionException.java,v 1.1 2002/11/25 11:00:52 nmcl Exp $
 * @since 1.0.
 */

public class NoTransactionException extends WSASException
{
    
    public NoTransactionException ()
    {
	super();
    }

    public NoTransactionException (String s)
    {
	super(s);
    }

    public NoTransactionException (String s, int errorcode)
    {
	super(s, errorcode);
    }
 
}

