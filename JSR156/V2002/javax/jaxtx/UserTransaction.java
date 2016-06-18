package javax.jaxtx;

import javax.jaxtx.completionstatus.CompletionStatus;
import javax.jaxtx.status.Status;

import javax.jaxtx.exceptions.SystemException;
import javax.jaxtx.exceptions.NestingNotAllowedException;
import javax.jaxtx.exceptions.WrongStateException;
import javax.jaxtx.exceptions.NoTransactionException;
import javax.jaxtx.exceptions.NoPermissionException;
import javax.jaxtx.exceptions.InvalidTimeoutException;
import javax.jaxtx.exceptions.InvalidTransactionException;
import javax.jaxtx.exceptions.ProtocolViolationException;

import org.w3c.dom.Document;

/**
 * The transaction interface is split into two:
 *
 * (i) the client component, which is typically only interested
 * in starting and ending a transaction session.
 *
 * (ii) the participant component, which is typically only
 * interested in registering a participant with the transaction.
 *
 * The UserTransaction is the interface that most users (e.g., clients
 * and services) will see. This isolates them from any underlying protocol
 * specific aspects of the implementation they are using.
 *
 * If required, an underlying implementation may enhance this interface
 * and provide additional methods to users. It is envisioned that users
 * will cast to the underlying implementation or use reflection.
 *
 * It is expected that UserTransactions will be made available via
 * JNDI and that there should be the possibility for a user to
 * locate all possible transaction implementations to determine which
 * one it requires.
 */

public interface UserTransaction
{

    /**
     * Returns parse-able information about the protocol this
     * transaction implements.
     *
     * @exception SystemException Thrown if an error occurs.
     * @return the XML configuration document.
     */

    public org.w3c.dom.Document configuration () throws SystemException;

    /**
     * Start a new transaction activity and associate it with the current
     * thread. The timeout parameter is used to terminate the transaction after
     * a specified period if it has not already begun to terminate.
     *
     * If a transaction is already associated with the current thread and the
     * transaction implementation does not support nesting, then the
     * NestingNotAllowedException is thrown.
     *
     * @param int timeout The timeout (in seconds) to associate with all
     * subsequently created transactions. This value must be 0 or greater.
     *
     * @exception WrongStateException Thrown if there is a transaction
     * already associated with the invoking thread and it is in a state that
     * does not allow a sibling transaction to be created.
     * @exception NestingNotAllowedException Thrown if there is a transaction
     * already associated with the invoking thread and nested transactions are
     * not supported.
     * @exception InvalidTimeoutException Thrown if the timeout value is
     * invalid within the scope of this transaction implementation. For
     * example, so implementations may not support timeouts at all.
     * @exception SystemException Thrown if some other error occurs.
     */

    public void start (int timeout) throws WrongStateException, NestingNotAllowedException, InvalidTimeoutException, SystemException;

    /**
     * End the transaction in the current completion status. If no
     * completion status has been set for the transaction, then the default
     * status of CompletionStatusCancel is used.
     *
     * @exception NoTransactionException Thrown if there is no transaction
     * associated with the invoking thread.
     * @exception InvalidTransactionException Thrown if the transaction is
     * invalid in the current execution environment.
     * @exception WrongStateException Thrown if the current transaction is
     * not in a state that allows it to end, or end in the required
     * completion state. For example, it is already terminating on behalf of
     * a different thread.
     * @exception NoPermissionException Thrown if the invoking thread does
     * not have permission to terminate the transaction.
     * @exception ProtocolViolationException Thrown if an error occurs which
     * violates the normal termination of the protocol. For example, a
     * heuristic outcome from a two-phase commit protocol would fall into
     * this category.
     * @exception SystemException Thrown if some other error occurs.
     *
     * @return the result of terminating the transaction. This may be null and
     * in which case it implies that the transaction terminated successfully
     * in the compeletion status required.
     */

    public Outcome end () throws NoTransactionException, InvalidTransactionException, WrongStateException, NoPermissionException, ProtocolViolationException, SystemException;

    /**
     * End the transaction in the specified completion status. If the
     * completion status is null then the last completion status set for this
     * transaction is used, or CompletionStatusCancel.
     *
     * @param CompletionStatus endStatus The completion status that the
     * transaction should attempt to terminate in.
     * @exception NoTransactionException Thrown if there is no transaction
     * associated with the invoking thread.
     * @exception InvalidTransactionException Thrown if the transaction is
     * invalid in the current execution environment.
     * @exception WrongStateException Thrown if the current transaction is
     * not in a state that allows it to end, or end in the required
     * completion state. For example, it is already terminating on behalf of
     * a different thread.
     * @exception NoPermissionException Thrown if the invoking thread does
     * not have permission to terminate the transaction.
     * @exception ProtocolViolationException Thrown if an error occurs which
     * violates the normal termination of the protocol. For example, a
     * heuristic outcome from a two-phase commit protocol would fall into
     * this category.
     * @exception SystemException Thrown if some other error occurs.
     *
     * @return the result of terminating the transaction. This may be null and
     * in which case it implies that the transaction terminated successfully
     * in the compeletion status required.
     */

    public Outcome end (CompletionStatus endStatus) throws NoTransactionException, InvalidTransactionException, WrongStateException, ProtocolViolationException, NoPermissionException, SystemException;

    /**
     * Set the timeout to be associated with all subsequently created
     * transactions. A default value of 0 is automatically associated with
     * each thread and this means that no application specified timeout is
     * set for transactions.
     *
     * @param int timeout The timeout (in seconds) to associate with all
     * subsequently created transactions. This value must be 0 or greater.
     *
     * @exception InvalidTimeoutException Thrown if the timeout value provided
     * is negative, too large, or if timeouts are simply not supported by
     * the transaction implementation.
     * @exception SystemException Thrown if any other error occurs.
     */

    public void setTimeout (int timeout) throws InvalidTimeoutException, SystemException;
    
    /**
     * Get the timeout value currently associated with transactions.
     *
     * @exception SystemException Thrown if any error occurs.
     *
     * @return the timeout value in seconds, or 0 if no application specified
     * timeout has been provided.
     */

    public int getTimeout () throws SystemException;

    /**
     * Set the termination status for the current transaction, if any.
     *
     * @param CompletionStatus endStatus The state in which the transaction
     * coordinator should attempt to terminate the transaction. This may
     * be one of the default values provided by JAXTX or may be extended in
     * an implementation specific manner.
     *
     * @exception NoTransactionException Thrown if there is no transaction
     * associated with the invoking thread.
     * @exception WrongStateException Thrown if the completion status is
     * incompatible with the current state of the transaction.
     * @exception SystemException Thrown if any other error occurs.
     */

    public void setCompletionStatus (CompletionStatus endStatus) throws NoTransactionException, WrongStateException, SystemException;

    /**
     * Get the completion status currently associated with the transaction.
     *
     * @exception NoTransactionException Thrown if there is no transaction
     * associated with the current thread.
     * @exception SystemException Thrown if any other error occurs.
     *
     * @return the termination status for the current transaction, if any.
     */

    public CompletionStatus getCompletionStatus () throws NoTransactionException, SystemException;

    /**
     * Get an opaque representation of the current transaction hierarchy.
     *
     * @exception SystemException Thrown if any error occurs.
     *
     * @return the token representing the current context, if any, and null
     * otherwise.
     */

    public TxContext getTransactionContext () throws SystemException;

    /**
     * Suspend the current transaction from this thread and return the token
     * representing the context, if any, and null otherwise. Once called, the
     * thread will have no transaction associated with it.
     *
     * @exception SystemException Thrown if any error occurs.
     *
     * @return the token representing the current context, if any, and null
     * otherwise.
     */

    public TxContext suspend () throws SystemException;

    /**
     * Given a token representing a context, associate it with the current
     * thread of control. This will implicitly disassociate the thread from any
     * transaction that it may already be associated with. If the parameter is
     * null then the thread is associated with no transaction.
     *
     * @param TxContext tx The transaction to associate with this thread. This
     * may be null in which case the current thread becomes associated with
     * no transaction.
     *
     * @exception InvalidTransactionException Thrown if the transaction handle
     * is invalid in this context.
     * @exception SystemException Thrown if any other error occurs.
     */

    public void resume (TxContext tx) throws InvalidTransactionException, SystemException;

    /**
     * @exception SystemException Thrown if any error occurs.
     *
     * @return the status of the current transaction, if any.
     */

    public Status status () throws SystemException;

    /**
     * @exception SystemException Thrown if any error occurs.
     *
     * @return the transaction identity. If there is no transaction associated
     * with the thread, then null is returned. In order that transaction ids
     * are guaranteed to be unique across different models, the identity is
     * required to be prefixed with the package name, e.g.,
     * "javax.jaxtx.model.jta.1234".
     */

    public String globalIdentity () throws SystemException;

    /**
     * Typically used for debugging purposes.
     *
     * @exception SystemException Thrown if any error occurs.
     *
     * @return a user-friendly name for this transaction. If there is no
     * transaction associated with the current thread, then null is returned.
     * This value may be the same as that returned by globalIdentity.
     */

    public String transactionName () throws SystemException;
 
}
