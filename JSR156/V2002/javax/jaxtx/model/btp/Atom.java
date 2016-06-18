package javax.jaxtx.model.btp;

import javax.jaxtx.UserTransaction;

import javax.jaxtx.exceptions.WrongStateException;
import javax.jaxtx.exceptions.InvalidParticipantException;
import javax.jaxtx.exceptions.NoTransactionException;
import javax.jaxtx.exceptions.InvalidTransactionException;
import javax.jaxtx.exceptions.SystemException;

/**
 * The Atom extension of the basic UserTransaction interface.
 *
 * The Cohesion and Atom implementations may co-exist within the
 * same address space.
 */

public interface Atom extends UserTransaction
{

    /**
     * Prepare the current Atom. Once prepared, the Atom can only move into
     * either a cancelled or confirmed state.
     *
     * @param Qualifier[] qualifiers Any additional information.
     * @exception WrongStateException Thrown if the state of the inferior is such that
     *            it cannot prepare.
     * @exception InvalidTransactionException Thrown if the atom identity is invalid
     *            (e.g., refers to an unknown inferior.)
     * @exception HazardException Thrown if upon preparing, the inferior finds that
     *            some of its enlisted inferiors have return statuses which
     *            mean it cannot determine what the result of issuing prepare
     *            to them has been.
     * @exception MixedException Thrown if upon preparing, the inferior finds that
     *            some of its enlisted inferiors have return statuses which
     *            mean some of them cancelled and some of them confirmed.
     * @exception NoTransactionException Thrown if there is no transaction associated
     *            with the invoking thread.
     * @exception SystemException Thrown if some other error occurred.
     *
     * @return the vote.
     * @see javax.jaxtx.model.btp.Vote
     */

    public Vote prepare (Qualifier[] qualifiers) throws WrongStateException, InvalidTransactionException, HazardException, MixedException, NoTransactionException, SystemException;

    /**
     * Confirm the current atom. The atom must have been previously prepared.
     * Once confirmed, the atom is terminated.
     *
     * @param Qualifier[] qualifiers Any additional information.
     * @exception WrongStateException Thrown if the state of the atom is such that
     *            it cannot confirm.
     * @exception InvalidTransactionException Thrown if the inferior identity is invalid
     *            (e.g., refers to an unknown inferior.)
     * @exception HazardException Thrown if upon preparing, the atom finds that
     *            some of its enlisted inferiors have return statuses which
     *            mean it cannot determine what the result of issuing confirm
     *            to them has been.
     * @exception MixedException Thrown if upon preparing, the inferior finds that
     *            some of its enlisted inferiors have return statuses which
     *            mean some of them cancelled and some of them confirmed.
     * @exception NoTransactionException Thrown if there is no transaction associated
     *            with the invoking thread.
     * @exception TransactionCancelledException Thrown if the inferior cancels
     * rather than confirms.
     * @exception SystemException Thrown if some other error occurred.
     */

    public void confirm (Qualifier[] qualifiers) throws WrongStateException, InvalidTransactionException, HazardException, MixedException, NoTransactionException, TransactionCancelledException, SystemException;

    /**
     * Cancel the current atom. The atom may have been previously prepared.
     * Once cancelled, the atom is terminated.
     *
     * @param Qualifier[] qualifiers Any additional information.
     * @exception WrongStateException Thrown if the state of the atom is such that
     *            it cannot cancel.
     * @exception InvalidTransactionException Thrown if the inferior identity is invalid
     *            (e.g., refers to an unknown inferior.)
     * @exception HazardException Thrown if upon preparing, the inferior finds that
     *            some of its enlisted inferiors have return statuses which
     *            mean it cannot determine what the result of issuing cancel
     *            to them has been.
     * @exception MixedException Thrown if upon preparing, the inferior finds that
     *            some of its enlisted inferiors have return statuses which
     *            mean some of them cancelled and some of them confirmed.
     * @exception InferiorConfirmedException Thrown if the inferior confirms
     * rather than cancels.
     * @exception NoTransactionException Thrown if there is no transaction associated
     *            with the invoking thread.
     * @exception SystemException Thrown if some other error occurred.
     */

    public void cancel (Qualifier[] qualifiers) throws WrongStateException, InvalidTransactionException, HazardException, MixedException, NoTransactionException, TransactionConfirmedException, SystemException;

}
