package javax.jaxtx.model.btp;

import javax.jaxtx.UserTransaction;
import javax.jaxtx.Outcome;

import javax.jaxtx.model.btp.StatusItem;

import javax.jaxtx.exceptions.WrongStateException;
import javax.jaxtx.exceptions.InvalidParticipantException;
import javax.jaxtx.exceptions.NoTransactionException;
import javax.jaxtx.exceptions.InvalidTransactionException;
import javax.jaxtx.exceptions.SystemException;

/**
 * A Cohesion extension of the basic UserTransaction.
 *
 * The Cohesion and Atom implementations may co-exist within the
 * same address space.
 */

public interface Cohesion extends UserTransaction
{

    /**
     * Prepare the transaction and have it call prepare on
     * the set of inferiors specified. The cohesion may be prepared
     * any number of times during its lifetime, although a single participant
     * can only be prepared once.
     *
     * @param String[] participantIds The list of inferiors that are to be prepared.
     * @param Qualifier[] qualifiers Any additional qualifiers that may affect
     *                    the prepare.
     * @exception InvalidTransactionException Thrown if the transaction identity is invalid.
     * @exception WrongStateException Thrown if the transaction is in a state that does
     *                       not allow it to issue prepare.
     * @exception HazardException Thrown if the inferiors generate a hazard heuristic.
     * @exception MixedException Thrown if the inferiors generate a mixed heuristic.
     * @exception InvalidParticipantException Thrown if the inferiors list is invalid.
     * @exception TransactionCancelledException Thrown if all inferiors cancel
     * during prepare.
     * @exception NoTransactionException Thrown if there is no transaction associated
     *            with the invoking thread.
     * @exception SystemException Thrown in the event of a general fault.
     *
     * @return a status value for each inferior.
     */

    public StatusItem[] prepare_inferiors (String[] participantIds, Qualifier[] qualifiers) throws InvalidTransactionException, WrongStateException, HazardException, MixedException, InvalidParticipantException, TransactionCancelledException, NoTransactionException, SystemException;

    /**
     * Cancel the transaction and have it call cancel on
     * the set of inferiors specified. The cohesion may have cancel
     * called any number of times during its lifetime.
     *
     * @param String[] participantIds The list of inferiors that are to be cancelled.
     * @param Qualifier[] qualifiers Any additional qualifiers that may affect
     *                    the cancel.
     * @exception WrongStateException Thrown if the transaction is in a state that does
     *                       not allow it to issue cancel.
     * @exception InvalidTransactionException Thrown if the transaction identity is invalid.
     * @exception HazardException Thrown if the inferiors generate a hazard heuristic.
     * @exception MixedException Thrown if the inferiors generate a mixed heuristic.
     * @exception InvalidParticipantException Thrown if the inferior list is invalid.
     * @exception NoTransactionException Thrown if there is no transaction associated
     *            with the invoking thread.
     * @exception TransactionConfirmedException Thrown if all inferiors confirm
     * during cancel.
     * @exception SystemException Thrown in the event of a general fault.
     *
     * @return a status value for each inferior.
     */

    public StatusItem[] cancel_inferiors (String[] participantIds, Qualifier[] qualifiers) throws WrongStateException, InvalidTransactionException, HazardException, MixedException, InvalidParticipantException, NoTransactionException, TransactionConfirmedException, SystemException;

    /**
     * Confirm the transaction and have it call confirm on
     * the set of inferiors specified. Once confirmed, the cohesion is
     * terminated.
     *
     * @param Qualifier[] qualifiers Any additional qualifiers that may affect
     *                    the confirm.
     * @exception WrongStateException Thrown if the transaction is in a state that does
     *                       not allow it to issue confirm.
     * @exception InvalidTransactionException Thrown if the transaction identity is invalid.
     * @exception HazardException Thrown if the inferiors generate a hazard heuristic.
     * @exception MixedException Thrown if the inferiors generate a mixed heuristic.
     * @exception NoTransactionException Thrown if there is no transaction associated
     *            with the invoking thread.
     * @exception TransactionCancelledException Thrown if all inferiors cancel
     * during confirm.
     * @exception SystemException Thrown in the event of a general fault.
     *
     * @return a status value for each inferior or null if they all confirmed.
     */

    public StatusItem[] confirm_transaction (Qualifier[] qualifiers) throws WrongStateException, InvalidTransactionException, HazardException, MixedException, NoTransactionException, TransactionCancelledException, SystemException;

    /**
     * Cancel the transaction and have it call confirm on
     * the set of inferiors specified. Once cancelled in this manner, the
     * cohesion is terminated.
     *
     * @param Qualifier[] qualifiers Any additional qualifiers that may affect
     *                    the confirm.
     * @exception WrongStateException Thrown if the transaction is in a state that does
     *                       not allow it to issue cancel.
     * @exception InvalidTransactionException Thrown if the transaction identity is invalid.
     * @exception HazardException Thrown if the inferiors generate a hazard heuristic.
     * @exception MixedException Thrown if the inferiors generate a mixed heuristic.
     * @exception NoTransactionException Thrown if there is no transaction associated
     *            with the invoking thread.
     * @exception TransactionConfirmedException Thrown if all inferiors confirm
     * during cancel.
     * @exception SystemException Thrown in the event of a general fault.
     *
     * @return a status value for each inferior or null if they all cancelled.
     */

    public StatusItem[] cancel_transaction (Qualifier[] qualifiers) throws WrongStateException, InvalidTransactionException, HazardException, MixedException, NoTransactionException, TransactionConfirmedException, SystemException;

}
