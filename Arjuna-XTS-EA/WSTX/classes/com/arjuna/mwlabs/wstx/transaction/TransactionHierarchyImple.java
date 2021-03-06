/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: TransactionHierarchyImple.java,v 1.1 2002/11/25 11:00:54 nmcl Exp $
 */

package com.arjuna.mwlabs.wstx.transaction;

import com.arjuna.mw.wstx.transaction.TransactionHierarchy;
import com.arjuna.mw.wstx.transaction.TransactionHandle;

import com.arjuna.mw.wsas.activity.ActivityHierarchy;

/**
 * This class represents a handle on a stack of transactions.
 * It should only be used for suspending and resuming the
 * thread-to-transaction association.
 *
 * The transaction at the top of the stack is the current transaction.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: TransactionHierarchyImple.java,v 1.1 2002/11/25 11:00:54 nmcl Exp $
 * @since 1.0.
 */

public class TransactionHierarchyImple implements TransactionHierarchy
{

    public TransactionHierarchyImple (ActivityHierarchy hier)
    {
	_hier = hier;
    }
    
    /**
     * @return the number of transactions associated with this stack.
     */

    public int size ()
    {
	return _hier.size();
    }
    
    /**
     * @return whether or not this is a valid context. If a single entry in
     * the context is invalid then we assume the entire context is.
     */

    public boolean valid ()
    {
	return _hier.valid();
    }

    /**
     * @param the specific transaction in the stack.
     * @exception IndexOutOfBoundsException Thrown if the transaction number
     * is too large or small.
     * @return the transaction handle.
     */

    public TransactionHandle transaction (int index) throws IndexOutOfBoundsException
    {
	return new TransactionHandleImple(_hier.activity(index));
    }

    /**
     * @return the current transaction in the context.
     */

    public TransactionHandle current ()
    {
	return new TransactionHandleImple(_hier.current());
    }

    /**
     * Obtain a copy of this context. Although the context may be shared
     * between different threads, the same instance of the context should
     * not be.
     *
     * @return the context copy.
     */

    public TransactionHierarchy copy ()
    {
	return new TransactionHierarchyImple(_hier.copy());
    }

    /**
     * Overrides Object.equals
     *
     * Two contexts are equal if both hierarchies are identical.
     */

    public boolean equals (Object obj)
    {
	if (obj == null)
	    return false;
	else
	{
	    if (obj instanceof TransactionHierarchyImple)
		return _hier.equals(((TransactionHierarchyImple) obj)._hier);
	    else
		return false;
	}
    }

    /**
     * Although users won't typically care what the underlying implementation
     * of a context is, they will need to do comparisons.
     * So, although this method is provided by Java.Object we have it here
     * to ensure that we don't forget to implement it!
     */

    public int hashCode ()
    {
	return _hier.hashCode();
    }

    public final ActivityHierarchy activityHierarchy ()
    {
	return _hier;
    }
    
    private ActivityHierarchy _hier;
    
}
