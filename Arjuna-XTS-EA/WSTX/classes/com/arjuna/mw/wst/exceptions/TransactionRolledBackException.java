/*
 * Copyright (C) 2003,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: TransactionRolledBackException.java,v 1.1 2003/02/03 11:01:26 nmcl Exp $
 */

package com.arjuna.mw.wst.exceptions;

/**
 * Thrown if the transaction rolls back rather than commits.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: TransactionRolledBackException.java,v 1.1 2003/02/03 11:01:26 nmcl Exp $
 * @since 1.0.
 */

public class TransactionRolledBackException extends Exception
{
    
    public TransactionRolledBackException ()
    {
	super();
    }

    public TransactionRolledBackException (String s)
    {
	super(s);
    }

}

