package javax.jaxtx.compensation;

/**
 * This is the interface that container uses to demarcate transactions.
 *
 * The CompensationTransactionManager does not represent a specific
 * transaction, but rather is responsible for providing access to an implicit
 * per-thread transaction context.
 * 
 * @version 1.0
 *
 * $Id: CompensationTransactionManager.java,v 1.1 2004/06/22 10:07:59 nmcl Exp $
 */

public interface CompensationTransactionManager
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

    public void confirm () throws CompensationTransactionCancelledException, UnknownCompensationTransactionException, SystemException;

    /**
     * If the CT must undo its work then the cancel method is used. Any
     * participants that can compensate are forced to do so.
     *
     * @exception UnknownCompensationTransactionException thrown if the system does
     * not recognize the transaction. This may occur if the transaction has previously
     * completed.
     * @exception SystemException thrown if some other error occurs.
     */

    public void cancel () throws UnknownCompensationTransactionException, SystemException;

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

    public void complete () throws UnknownCompensationTransactionException, SystemException;

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

    /**
     * The resume method can be used to (re-)associate a thread with a 
     * transaction(s). Prior to association, the thread is
     * disassociated with any transaction(s) with which it may be currently
     * associated.
     *
     * @param CompensationTransaction tx The transaction to associated. If
     * it is null, then the thread is associated with no transaction.
     *
     * @exception UnknownCompensationTransactionException thrown if
     * the transaction is invalid in the scope of
     * the invoking thread.
     * @exception SystemException thrown if some other error occurs.
     */

    public void resume (CompensationTransaction tx) throws UnknownCompensationTransactionException, SystemException;

    /**
     * A thread of control may require periods of non-transactionality so that
     * it may perform work that is not associated with a specific transaction.
     * In order to do this it is necessary to disassociate the thread from any
     * transactions. The suspend method accomplishes this, returning a
     * reference to the transaction. The thread is
     * then no longer associated with any transaction.
     *
     * @return the compensation transaction reference, or null if the
     * thread was not associated with a transaction.
     *
     * @exception SystemException thrown if some other error occurs.
     */

    public CompensationTransaction suspend () throws SystemException;
    
    /**
     * The currentTransaction method returns a reference for the current
     * transaction, or null if there is none. Unlike suspend, this method does
     * not disassociate the current thread from the transaction(s). This can
     * be used to enable multiple threads to execute within the scope of the
     * same transaction.
     * 
     * @return the compensation transaction reference, or null if the
     * thread was not associated with a transaction.
     *
     * @exception SystemException thrown if some other error occurs.
     */

    public CompensationTransaction currentTransaction () throws SystemException;
    
}
