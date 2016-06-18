package javax.jaxtx;

/**
 * This class represents a handle on a stack of transactions.
 * It should only be used for suspending and resuming the
 * thread-to-transaction association.
 *
 * The transaction at the top of the stack is the current transaction.
 */

public interface TxContext
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

    public TxHandle transaction (int index) throws IndexOutOfBoundsException;

    /**
     * @return the current transaction in the context.
     */

    public TxHandle current ();

    /**
     * Obtain a copy of this context. Although the context may be shared
     * between different threads, the same instance of the context should
     * not be.
     *
     * @return the context copy.
     */

    public TxContext copy ();

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
