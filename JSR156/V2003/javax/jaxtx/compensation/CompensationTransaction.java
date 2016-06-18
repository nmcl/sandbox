package javax.jaxtx.compensation;

/**
 * This is the interface that the transaction service implementation exposes
 * in order to allow compensation participants to be enrolled. Unlike when
 * terminating a transaction through the UserCompensationTransaction or
 * CompensationTransactionManager interfaces, terminating the transaction
 * via this interface does not change the thread-to-transaction association.
 *
 * The CompensationTransaction represents a specific transaction.
 *
 * @version 1.0
 *
 * $Id: CompensationTransaction.java,v 1.3 2005/10/21 09:26:56 nmcl Exp $
 */

public interface CompensationTransaction
{

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
     * been received. This does not signal the end of the transaction,
     * i.e., it still needs to be either cancelled or confirmed.
     *
     * @exception UnknownCompensationTransactionException thrown if the system does
     * not recognize the transaction. This may occur if the transaction has previously
     * completed.
     * @exception SystemException thrown if some other error occurs.
     */

    public void complete () throws UnknownCompensationTransactionException, SystemException;

    /**
     * Enlist a compensation with the compensation transaction currently
     * associated with this thread. Each compensation participant can only
     * be registered once per transaction, but may perform compensation on
     * behalf of multiple business services.
     *
     * @param CompensationParticipant bap the compensation to register.
     *
     * @exception WrongStateException thrown if the transaction is in a state
     * where no further compensations can be registered. The service must catch
     * this exception and deal with it in a way that does not compromise the
     * integrity of the application.
     * @exception UnknownCompensationTransactionException thrown in the event
     * that the transaction service implementation does not know about the
     * specific compensation transaction represented by this instance.
     * @exception AlreadyRegisteredException thrown if the transaction has 
     * already seen this participant in the registration phase.
     * @exception SystemException thrown if any other failure occurs.
     *
     * @return the CompensationParticipantManager for this transaction, or null
     * if there is no such manager.
     */

    public CompensationParticipantManager enlistParticipant (CompensationParticipant bap) throws WrongStateException, UnknownCompensationTransactionException, AlreadyRegisteredException, SystemException;

    /**
     * Delist the compensation with the compensation transaction currently
     * associated with this thread. In doing so, the business service or
     * container is indicating to the coordinator that the work encapsulated
     * by this compensation is of no interest to the outcome of the
     * transaction.
     *
     * @param CompensationParticipant bap the compensation to register.
     *
     * @exception WrongStateException thrown if the transaction is in a state
     * where compensations cannot be un-registered. The service must catch
     * this exception and deal with it in a way that does not compromise the
     * integrity of the application.
     * @exception UnknownCompensationTransactionException thrown in the event
     * that the transaction service implementation does not know about the
     * specific compensation transaction represented by this instance.
     * @exception NotRegisteredException thrown if the transaction has 
     * not seen this participant in the registration phase.
     * @exception SystemException thrown if any other failure occurs.
     */

    public void delistParticipant (CompensationParticipant bap) throws WrongStateException, UnknownCompensationTransactionException, NotRegisteredException, SystemException;

    /**
     * A business service/container may set the outcome of the compensation
     * transaction such that it must either cancel or compensate, depending
     * upon the state indicated by other participants. The central notion
     * is that the transaction cannot complete the work successfully and must
     * somehow cancel it.
     *
     * @exception WrongStateException thrown if the transaction is no longer
     * in an active state.
     * @exception SystemException thrown if any other failure occurs.
     */

    public void setUndoOnly () throws WrongStateException, SystemException;

    /**
     * This method is used to obtain the current status of the transaction.
     *
     * @return the status of this transaction.
     *
     * @see javax.jaxtx.compensation.Status
     *
     * @exception SystemException thrown if any error occurs.
     */

    public int status () throws SystemException;
    
    /**
     * A given transaction instance may be marked as invalid within the
     * scope of a given application.
     *
     * @return true if the transaction is valid within the context of
     * this implementation, false otherwise.
     */

    public boolean valid ();

    /**
     * It is often necessary to be able to compare transactions, e.g., in some
     * protocol implementations, participants may only be registered once
     * with a transaction and this operation can be used to determine whether
     * or not a container has already seen this specific transaction.
     *
     * @param Object o the transaction with which to compare this instance.
     *
     * @return true if the parameter represents the same transaction as
     * the target object, false otherwise.
     */

    public boolean equals (Object o);

}
