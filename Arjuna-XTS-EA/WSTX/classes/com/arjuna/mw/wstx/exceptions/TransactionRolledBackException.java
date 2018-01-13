/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: TransactionRolledBackException.java,v 1.1 2002/11/25 11:00:52 nmcl Exp $
 */

package com.arjuna.mw.wstx.exceptions;

import com.arjuna.mw.wsas.exceptions.WSASException;

/**
 * Thrown if the transaction rolls back rather than commits.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: TransactionRolledBackException.java,v 1.1 2002/11/25 11:00:52 nmcl Exp $
 * @since 1.0.
 */

public class TransactionRolledBackException extends WSASException
{
    
    public TransactionRolledBackException ()
    {
	super();
    }

    public TransactionRolledBackException (String s)
    {
	super(s);
    }

    public TransactionRolledBackException (String s, int errorcode)
    {
	super(s, errorcode);
    }
 
}

