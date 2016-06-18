package javax.jaxtx.compensation;

/**
 * This is the interface that participants within a compensation
 * transaction must support. The coordinator then invokes the various
 * participants depending upon whether or not it completes successfully.
 * A compensation participant is required to be able to undo any work that
 * was performed on behalf of some business service. How it does this is
 * implementation (and possibly protocol) specific. There is no assumption
 * as to how many business services a given participant may act on behalf of,
 * i.e., it could be a one-to-one relationship or one-to-many.
 *
 * If a participant is required to undo work or compensate then it is possible
 * that such cancellation may fail. Depending upon the implementation protocol
 * this failure may need to be communicated back to the coordinator and in
 * which case, the comepensation participant may need to remember certain
 * information about the work is failed to undo until the coordinator (or
 * more likely a system administrator) has been able to manually resolve the
 * situation. Again, however, this may be dependant upon the specific
 * transaction protocol. 
 *
 * @version 1.0
 *
 * $Id: CompensationParticipant.java,v 1.1 2004/06/22 10:07:58 nmcl Exp $
 */

public interface CompensationParticipant
{

    /**
     * The compensation transaction has terminated successfully and no
     * further work is required by the participants. When a participant
     * receives this invocation it can discard any information it may have
     * maintained in the event a compensation may have been required.
     *
     * @exception WrongStateException the participant is not in a state
     * where this invocation is allowed.
     * @exception UnknownCompensationTransactionException the participant does
     * not know about the transaction coordinator. This may occur if the
     * participant has been garbage collected.
     * @exception SystemException thrown if any other error occurs.
     */

    public void complete () throws WrongStateException, UnknownCompensationTransactionException, SystemException;

    /**
     * The compensation transaction has terminated unsuccessfully and the
     * participant is expected to undo the work it performed. The
     * participant had not previously indicated to the coordinator that it
     * was finished work (i.e., had not completed), so compensation is not
     * necessary.
     *
     * @exception WrongStateException the participant is not in a state
     * where this invocation is allowed.
     * @exception UnknownCompensationTransactionException the participant does
     * not know about the transaction coordinator. This may occur if the
     * participant has been garbage collected.
     * @exception SystemException thrown if any other error occurs.
     */

    public void cancel () throws WrongStateException, UnknownCompensationTransactionException, SystemException;

    /**
     * The compensation transaction has terminated unsuccessfully and the
     * participant is expected to compensate for the work it performed.
     * The service/participant previously informed the coordinator that it had
     * finished work but could compensate later if required, so it is now
     * requested to do so.
     *
     * @exception WrongStateException the participant is not in a state
     * where this invocation is allowed.
     * @exception UnknownCompensationTransactionException the participant does
     * not know about the transaction coordinator. This may occur if the
     * participant has been garbage collected.
     * @exception SystemException thrown if any other error occurs.
     */

    public void compensate () throws WrongStateException, UnknownCompensationTransactionException, SystemException;

    /**
     * The participant generated an error message when either compensating or
     * terminating. Once this has been resolved, the coordinator will inform
     * the participant, which can forget about the transaction. Until that
     * happens, it must retain information about the transaction.
     *
     * @exception WrongStateException the participant is not in a state
     * where this invocation is allowed.
     * @exception UnknownCompensationTransactionException the participant does
     * not know about the transaction coordinator. This may occur if the
     * participant has been garbage collected.
     * @exception SystemException thrown if any other error occurs.
     */

    public void forget () throws WrongStateException, UnknownCompensationTransactionException, SystemException;

    /**
     * This operation gives the current status of this participant.
     *
     * @return the status of this participant in the transaction.
     *
     * @see javax.jaxtx.compensation.Status
     *
     * @exception SystemException thrown if any error occurs.
     */

    public int status () throws SystemException;
    
}
