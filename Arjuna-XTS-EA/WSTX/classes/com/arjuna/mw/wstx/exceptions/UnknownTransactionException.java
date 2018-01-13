/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: UnknownTransactionException.java,v 1.1 2002/11/25 11:00:52 nmcl Exp $
 */

package com.arjuna.mw.wstx.exceptions;

import com.arjuna.mw.wsas.exceptions.WSASException;

/**
 * Thrown if the transaction is unknown.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: UnknownTransactionException.java,v 1.1 2002/11/25 11:00:52 nmcl Exp $
 * @since 1.0.
 */

public class UnknownTransactionException extends WSASException
{
    
    public UnknownTransactionException ()
    {
	super();
    }

    public UnknownTransactionException (String s)
    {
	super(s);
    }

    public UnknownTransactionException (String s, int errorcode)
    {
	super(s, errorcode);
    }
 
}

