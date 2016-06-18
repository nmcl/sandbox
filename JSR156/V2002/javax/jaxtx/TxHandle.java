package javax.jaxtx;

/**
 * TxHandle is used as a representation of a single transaction
 * when it is suspended from a running thread and may be later
 * resumed. The implementation of the token can be as lightweight
 * as required by the underlying implementation in order that it
 * can uniquely represent all transaction instances.
 *
 * Since this is a client-facing class, it is unlikely that the
 * application user will typically want to see the entire transaction
 * context in order to simply suspend it from the thread.
 */

public interface TxHandle
{
    /**
     * Although users won't typically care what the underlying implementation
     * of a context is, they will need to do comparisons.
     * So, although this method is provided by Java.Object we have it here
     * to ensure that we don't forget to implement it!
     *
     * Two instances are equal if the refer to the same transaction.
     */

    public boolean equals (Object obj);
    
    /**
     * Although users won't typically care what the underlying implementation
     * of a context is, they will need to do comparisons.
     * So, although this method is provided by Java.Object we have it here
     * to ensure that we don't forget to implement it!
     */

    public int hashCode ();

    /**
     * @return whether or not this is a valid handle.
     */

    public boolean valid ();

    /**
     * @return the transaction identifier.
     */

    public String tid ();

}
