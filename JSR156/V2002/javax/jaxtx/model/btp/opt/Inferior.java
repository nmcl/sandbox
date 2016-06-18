package javax.jaxtx.model.btp.opt;

import javax.jaxtx.model.btp.Qualifier;
import javax.jaxtx.model.btp.Vote;

import javax.jaxtx.exceptions.WrongStateException;
import javax.jaxtx.exceptions.SystemException;

import javax.jaxtx.model.btp.HazardException;
import javax.jaxtx.model.btp.MixedException;

/**
 * This is the interface that all inferiors must implement in order to be
 * driven through the two-phase completion protocol. Inferior implementations
 * may be factories handling many different instances; hence, all methods
 * require the identity of the inferior they are to work on.
 */

public interface Inferior
{

    /**
     * Prepare the inferior specified by the id. This is to allow
     * an implementation to put a manager/factory entity in the loop
     * to (possibly) improve performance, for example. So, the entity
     * that implements this interface may be wrapping multiple
     * implementation objects.
     *
     * @param String id The identity of the inferior to prepare.
     * @param Qualifier[] qualifiers Any additional information.
     * @exception InvalidInferiorException Thrown if the inferior identity is invalid
     *            (e.g., refers to an unknown inferior.)
     * @exception WrongStateException Thrown if the state of the inferior is such that
     *            it cannot prepare.
     * @exception HazardException Thrown if upon preparing, the inferior finds that
     *            some of its enlisted inferiors have return statuses which
     *            mean it cannot determine what the result of issuing prepare
     *            to them has been.
     * @exception MixedException Thrown if upon preparing, the inferior finds that
     *            some of its enlisted inferiors have return statuses which
     *            mean some of them cancelled and some of them confirmed.
     * @exception SystemException Thrown if some other error occurred.
     *
     * @return the vote.
     * @see javax.jaxtx.models.btp.Vote
     */
    
    public Vote prepare (String id, Qualifier[] qualifiers) throws SystemException, InvalidInferiorException, WrongStateException, HazardException, MixedException;

    /**
     * Confirm the inferior specified.
     *
     * @param String id The identity of the inferior to confirm.
     * @param Qualifier[] qualifiers Any additional information.
     * @exception InvalidInferiorException Thrown if the inferior identity is invalid
     *            (e.g., refers to an unknown inferior.)
     * @exception WrongStateException Thrown if the state of the inferior is such that
     *            it cannot confirm.
     * @exception HazardException Thrown if upon preparing, the inferior finds that
     *            some of its enlisted inferiors have return statuses which
     *            mean it cannot determine what the result of issuing confirm
     *            to them has been.
     * @exception MixedException Thrown if upon preparing, the inferior finds that
     *            some of its enlisted inferiors have return statuses which
     *            mean some of them cancelled and some of them confirmed.
     * @exception InferiorCancelledException Thrown if the inferior cancels
     * rather than confirms.
     * @exception SystemException Thrown if some other error occurred.
     */

    public void confirm (String id, Qualifier[] qualifiers) throws SystemException, InvalidInferiorException, WrongStateException, HazardException, MixedException, InferiorCancelledException;

    /**
     * Cancel the inferior specified.
     *
     * @param String id The identity of the inferior to cancel.
     * @param Qualifier[] qualifiers Any additional information.
     * @exception InvalidInferiorException Thrown if the inferior identity is invalid
     *            (e.g., refers to an unknown inferior.)
     * @exception WrongStateException Thrown if the state of the inferior is such that
     *            it cannot cancel.
     * @exception HazardException Thrown if upon preparing, the inferior finds that
     *            some of its enlisted inferiors have return statuses which
     *            mean it cannot determine what the result of issuing cancel
     *            to them has been.
     * @exception MixedException Thrown if upon preparing, the inferior finds that
     *            some of its enlisted inferiors have return statuses which
     *            mean some of them cancelled and some of them confirmed.
     * @exception InferiorConfirmedException Thrown if the inferior confirms
     * rather than cancels.
     * @exception SystemException Thrown if some other error occurred.
     * @return the final outcome.
     */

    public void cancel (String id, Qualifier[] qualifiers) throws SystemException, InvalidInferiorException, WrongStateException, HazardException, MixedException, InferiorConfirmedException;

    /**
     * Inform the specified inferior that it has generated a contradiction.
     *
     * @param String id The identity of the inferior.
     * @param Qualifier[] qualifiers Any additional qualifiers that may affect
     *                    the operation.
     * @exception InvalidInferiorException Thrown if the inferior identity is invalid.
     * @exception WrongStateException Thrown if the inferior is in an invalid state.
     * @exception SystemException Thrown in the event of a general fault.
     */

    public void contradiction (String id, Qualifier[] qualifiers) throws SystemException, InvalidInferiorException, WrongStateException;

    /**
     * @param String id The identity of the inferior.
     * @param Qualifier[] qualifiers Any additional qualifiers that may affect
     *                    the operation.
     * @exception InvalidInferiorException Thrown if the inferior identity is invalid.
     * @exception SystemException Thrown in the event of a general fault.
     * @return the current status of the specified inferior.
     * @see javax.jaxtx.models.btp.TwoPhaseStatus
     */

    public int status (String id, Qualifier[] qualifiers) throws SystemException, InvalidInferiorException;

}
