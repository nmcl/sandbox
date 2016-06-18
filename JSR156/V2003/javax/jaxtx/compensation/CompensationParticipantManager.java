package javax.jaxtx.compensation;

/**
 * When a Compensation Transaction participant registers with a transaction it
 * may get a reference to a service that lets it callback into the transaction
 * and drive any state-transitions for itself. This reference may represent
 * the same endpoint as the CompensationTransaction or may be an entirely
 * separate service.
 *
 * There is one instance of this interface per compensation participant.
 * Implementations of this interface are required to be serializable such that
 * they may be used in the event of failure and subsequent recovery. How a
 * given implementation ensures that a serializable transaction instance is
 * rebound upon recovery is beyond the scope of this specification.
 *
 * @version 1.0
 *
 * $Id: CompensationParticipantManager.java,v 1.2 2004/12/02 11:10:34 nmcl Exp $
 */

public interface CompensationParticipantManager extends java.io.Serializable
{

    /**
     * The participant wants to remove itself from the compensation
     * transaction. This may be equivalent to using the delist operation
     * on the corresponding compensation transaction.
     *
     * @throws WrongStateException thrown if the transaction is no longer
     * in a state where exiting is allowed.
     * @throws UnknownCompensationTransactionException thrown if the system
     * no longer has record about the transaction.
     * @throws UnknownParticipantException thrown if the system does not
     * know about this participant.
     * @throws SystemException thrown if some other error occurs.
     */

    public void exited () throws WrongStateException, UnknownCompensationTransactionException, UnknownParticipantException, SystemException;

    /**
     * The participant has completed its work and is ready for the
     * transaction to terminate.
     *
     * @throws WrongStateException thrown if the transaction is not in
     * a state where completion is possible.
     * @throws UnknownCompensationTransactionException thrown if the system
     * no longer has record about the transaction.
     * @throws UnknownParticipantException thrown if the system does not
     * know about this participant.
     * @throws SystemException thrown if some other error occurs.
     */

    public void completed () throws WrongStateException, UnknownCompensationTransactionException, UnknownParticipantException, SystemException;

    /**
     * A participant or recovery service acting on its behalf, may enquire
     * as to the recovery status of the transaction. If recovery is necessary
     * and has not been instigated on behalf of this participant, then this
     * invocation may institute the recovery process.
     *
     * @throws UnknownCompensationTransactionException thrown if the system
     * no longer has record about the transaction.
     * @throws SystemException thrown if some other error occurs.
     *
     * @return the current status of the transaction. The participant may
     * use this to optimize its own recovery processes.
     *
     * @see javax.jaxtx.compensation.Status
     */

    public int recover () throws UnknownCompensationTransactionException, SystemException;
    
}
