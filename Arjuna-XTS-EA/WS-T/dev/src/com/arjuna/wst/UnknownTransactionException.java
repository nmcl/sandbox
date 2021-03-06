/*
 * Copyright (C) 2003,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: UnknownTransactionException.java,v 1.1 2003/02/03 16:24:46 nmcl Exp $
 */

package com.arjuna.wst;

/**
 * Thrown if the transaction is unknown.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: UnknownTransactionException.java,v 1.1 2003/02/03 16:24:46 nmcl Exp $
 * @since 1.0.
 */

public class UnknownTransactionException extends Exception
{
    
    public UnknownTransactionException ()
    {
	super();
    }

    public UnknownTransactionException (String s)
    {
	super(s);
    }

}

