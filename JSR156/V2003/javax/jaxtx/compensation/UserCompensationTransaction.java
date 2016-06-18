package javax.jaxtx.compensation;

/**
 * This is the interface that allows Compensation transactions (CTs)
 * to be started and terminated.
 *
 * Importantly, a UserCompensationTransaction does not represent a specific
 * CT, but rather is responsible for providing access to an implicit
 * per-thread  transaction context; it is similar to the UserTransaction in
 * the JTA specification. Therefore, all of the methods
 * implicitly act on the current thread of control.
 *
 * @version 1.0
 *
 * $Id: UserCompensationTransaction.java,v 1.2 2004/09/16 10:06:26 nmcl Exp $
 */

public interface UserCompensationTransaction
{
    
    /**
     * Start a new compensation transaction. Upon success, this
     * operation associates the newly created transaction with the current
     * thread. A timeout may be associated with the transaction if a previous
     * call to setTimeout has been made.
     *
     * @exception NotSupportedException thrown if nesting of compensation
     * transactions is not supported.
     * @exception SystemException thrown if some other error occurs.
     */

    public void begin () throws NotSupportedException, SystemException;

    /**
     * The CT is normally terminated by the confirm method. This signals to
     * all registered participants that the CT has ended and no compensation
     * is required.
     *
     * @exception CompensationTransactionCancelledException thrown if the transaction
     * cancelled. This will happen if the timeout elapses prior to a call
     * to confirm.
     * @exception UnknownCompensationTransactionException thrown if the system does
     * not recognize the transaction. This may occur if the transaction has previously
     * completed.
     * @exception SystemException thrown if some other error occurs.
     */

    public void confirm () throws CompensationTransactionCancelledException, UnknownCompensationTransactionException, NoCompensationTransactionException, SystemException;

    /**
     * If the CT must undo its work then the cancel method is used. Any
     * participants that can compensate are forced to do so.
     *
     * @exception UnknownCompensationTransactionException thrown if the system does
     * not recognize the transaction. This may occur if the transaction has previously
     * completed.
     * @exception SystemException thrown if some other error occurs.
     */

    public void cancel () throws UnknownCompensationTransactionException, NoCompensationTransactionException, SystemException;

    /**
     * The user of a CT may inform all currently enlisted participants
     * that the work they are expected to do in the scope of the CT has
     * been received.
     *
     * @exception UnknownCompensationTransactionException thrown if the system does
     * not recognize the transaction. This may occur if the transaction has previously
     * completed.
     * @exception SystemException thrown if some other error occurs.
     */

    public void complete () throws UnknownCompensationTransactionException, NoCompensationTransactionException, SystemException;

    /**
     * @exception NoCompensationTransactionException Thrown if there is no transaction
     * associated with the calling thread.
     * @exception SystemException Thrown if any error occurs.
     *
     * @return the status of the current transaction.
     *
     * @see javax.jaxtx.compensation.Status
     */

    public int status () throws NoCompensationTransactionException, SystemException;

     /**
     * Returns the timeout value that will be associated with any newly created
     * compensation transactions on behalf of this thread. This may not be the
     * value associated with any currently running transactions. A value of 0 is
     * an implementation specific timeout.
     *
     * @exception SystemException thrown if some other error occurs.
     *
     * @return the timeout value associated with this thread.
     */

    public int getTimeout () throws SystemException;

    /**
     * Sets the timeout value that will be associated with any newly
     * created compensation transactions on behalf of this thread. This value
     * will be used if no other value is specified during a subsequent call
     * to begin. A value of 0 is an implementation specific timeout.
     *
     * @param int timeout The lifetime of the transaction. If it has not
     * terminated by the time this timeout elapses, then the coordinator
     * may cancel the transaction. A value of 0 means an implementation
     * specific timeout is associated with the transaction.
     *
     * @exception SystemException thrown if some other error occurs.
     *
     * @return the timeout value associated with this thread.
     */

    public void setTimeout (int timeout) throws SystemException;
    
}
