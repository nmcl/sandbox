/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: InvalidTransactionException.java,v 1.1 2002/11/25 11:00:52 nmcl Exp $
 */

package com.arjuna.mw.wstx.exceptions;

import com.arjuna.mw.wsas.exceptions.WSASException;

/**
 * Thrown if the transaction (or imported context) is invalid.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: InvalidTransactionException.java,v 1.1 2002/11/25 11:00:52 nmcl Exp $
 * @since 1.0.
 */

public class InvalidTransactionException extends WSASException
{

    public InvalidTransactionException ()
    {
	super();
    }

    public InvalidTransactionException (String s)
    {
	super(s);
    }

    public InvalidTransactionException (String s, int errorcode)
    {
	super(s, errorcode);
    }
    
}


