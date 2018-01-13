/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: TransactionCommittedException.java,v 1.1 2002/11/25 11:00:52 nmcl Exp $
 */

package com.arjuna.mw.wstx.exceptions;

import com.arjuna.mw.wsas.exceptions.WSASException;

/**
 * Thrown if the transaction commits rather than rolls back.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: TransactionCommittedException.java,v 1.1 2002/11/25 11:00:52 nmcl Exp $
 * @since 1.0.
 */

public class TransactionCommittedException extends WSASException
{
    
    public TransactionCommittedException ()
    {
	super();
    }

    public TransactionCommittedException (String s)
    {
	super(s);
    }

    public TransactionCommittedException (String s, int errorcode)
    {
	super(s, errorcode);
    }
 
}

