package javax.jaxtx;

import javax.jaxtx.status.Status;

import javax.jaxtx.exceptions.SystemException;

import org.w3c.dom.Document;

/**
 * The Outcome represents the final outcome of the transaction. Because
 * different transaction implementations will have different requirements
 * on what they can or cannot return, this interface is deliberately
 * generic.
 */

public interface Outcome
{

    /**
     * Give a name for this outcome.
     *
     * @exception SystemException Thrown if an error occurs.
     * @return some implementation specific name for the Outcome. Typically
     * this will be the only thing necessary to determine the transaction's
     * outcome, e.g., "RolledBack".
     */

    public String name () throws SystemException;

    /**
     * The state in which the transaction completed.
     *
     * @exception SystemException Thrown if an error occurs.
     * @return the final completion status of the transaction. Any additional
     * information (e.g., exception types) may be provided by the data method.
     */
 
    public Status completedStatus () throws SystemException;
 
    /**
     * Outcome specific information that can be used to determine finer
     * level details about the outcome instance.
     *
     * @exception SystemException Thrown if an error occurs.
     * @return completion specific data for this transaction. Examples include
     * the type of failure exception that was thrown by the implementation
     * (e.g., HeuristicMixed).
     */

    public org.w3c.dom.Document data () throws SystemException;
    
}
