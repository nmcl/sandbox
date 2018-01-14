/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: TransactionHierarchy.java,v 1.1 2002/11/25 11:00:53 nmcl Exp $
 */

package com.arjuna.mw.wstx.transaction;

/**
 * This class represents a handle on a stack of transactions.
 * It should only be used for suspending and resuming the
 * thread-to-transaction association.
 *
 * The transaction at the top of the stack is the current transaction.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: TransactionHierarchy.java,v 1.1 2002/11/25 11:00:53 nmcl Exp $
 * @since 1.0.
 */

public interface TransactionHierarchy
{
    /**
     * @return the number of transactions associated with this stack.
     */

    public int size ();
    
    /**
     * @return whether or not this is a valid context. If a single entry in
     * the context is invalid then we assume the entire context is.
     */

    public boolean valid ();

    /**
     * @param the specific transaction in the stack.
     * @exception IndexOutOfBoundsException Thrown if the transaction number
     * is too large or small.
     * @return the transaction handle.
     */

    public TransactionHandle transaction (int index) throws IndexOutOfBoundsException;

    /**
     * @return the current transaction in the context.
     */

    public TransactionHandle current ();

    /**
     * Obtain a copy of this context. Although the context may be shared
     * between different threads, the same instance of the context should
     * not be.
     *
     * @return the context copy.
     */

    public TransactionHierarchy copy ();

    /**
     * Overrides Object.equals
     *
     * Two contexts are equal if both hierarchies are identical.
     */

    public boolean equals (Object obj);

    /**
     * Although users won't typically care what the underlying implementation
     * of a context is, they will need to do comparisons.
     * So, although this method is provided by Java.Object we have it here
     * to ensure that we don't forget to implement it!
     */

    public int hashCode ();
    
}
