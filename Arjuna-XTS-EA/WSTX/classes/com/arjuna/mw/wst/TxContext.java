/*
 * Copyright (C) 2003,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: TxContext.java,v 1.3 2003/03/24 10:57:01 nmcl Exp $
 */

package com.arjuna.mw.wst;

/**
 * This class represents a handle on a stack of transactions.
 * It should only be used for suspending and resuming the
 * thread-to-transaction association.
 *
 * The transaction at the top of the stack is the current transaction.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: TxContext.java,v 1.3 2003/03/24 10:57:01 nmcl Exp $
 * @since XTS 1.0.
 */

public interface TxContext
{

    /**
     * @return true if the context is valid, false otherwise.
     */

    public boolean valid ();

    /**
     * @return true if the parameter represents the same context as
     * the target object, false otherwise.
     */

    public boolean equals (Object o);
    
}
