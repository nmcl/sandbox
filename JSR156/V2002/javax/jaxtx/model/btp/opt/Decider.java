package javax.jaxtx.model.btp.opt;

import javax.jaxtx.model.btp.Qualifier;
import javax.jaxtx.model.btp.StatusItem;

import javax.jaxtx.exceptions.WrongStateException;
import javax.jaxtx.exceptions.SystemException;

import javax.jaxtx.model.btp.HazardException;
import javax.jaxtx.model.btp.MixedException;

import javax.jaxtx.model.btp.opt.DeciderCancelledException;
import javax.jaxtx.model.btp.opt.DeciderConfirmedException;

/**
 * This is the interface that all cohesions provide in order that
 * they can be driven to completion. A root cohesion is in some ways
 * a superior but in others an inferior to some undefined entity
 * (e.g., the business logic). We place it within the superior
 * package because it's the root of any tree as far as we are concerned,
 * i.e., we have no other classes that are superior to it.
 */

public interface Decider
{

    /**
     * Prepare the inferior and have it call prepare on
     * the set of inferiors specified.
     *
     * @param String id The identity of the superior that the inferiors are
     *               enlisted with.
     * @param String[] atoms The list of inferiors that are to be prepared.
     * @param Qualifier[] qualifiers Any additional qualifiers that may affect
     *                    the prepare.
     * @exception InvalidDeciderException Thrown if the decider identity is invalid.
     * @exception InvalidInferiorException Thrown if the inferiors list is invalid.
     * @exception WrongStateException Thrown if the superior is in a state that does
     *                       not allow it to issue prepare.
     * @exception HazardException Thrown if the inferiors generate a hazard heuristic.
     * @exception MixedException Thrown if the inferiors generate a mixed heuristic.
     * @exception DeciderCancelledException Thrown if all inferiors cancel
     * during prepare.
     * @exception SystemException Thrown in the event of a general fault.
     *
     * @return a status value for each inferior.
     */

    public StatusItem[] prepare_inferiors (String id, String[] atoms, Qualifier[] qualifiers) throws SystemException, InvalidInferiorException, WrongStateException, HazardException, MixedException, InvalidDeciderException, DeciderCancelledException;

    /**
     * Confirm the inferior and have it call confirm on
     * the set of inferiors specified.
     *
     * @param String id The identity of the superior that the inferiors are
     *               enlisted with.
     * @param String[] atoms The list of inferiors that are to be confirmed.
     * @param Qualifier[] qualifiers Any additional qualifiers that may affect
     *                    the confirm.
     * @exception InvalidDeciderException Thrown if the decider identity is invalid.
     * @exception InvalidInferiorException Thrown if the inferior list is invalid.
     * @exception WrongStateException Thrown if the superior is in a state that does
     *                       not allow it to issue confirm.
     * @exception HazardException Thrown if the inferiors generate a hazard heuristic.
     * @exception MixedException Thrown if the inferiors generate a mixed heuristic.
     * @exception DeciderCancelledException Thrown if all inferiors cancel
     * during confirm.
     * @exception SystemException Thrown in the event of a general fault.
     *
     * @return a status value for each inferior.
     */

    public StatusItem[] confirm_inferiors (String id, String[] atoms, Qualifier[] qualifiers) throws SystemException, InvalidInferiorException, WrongStateException, HazardException, MixedException, InvalidDeciderException, DeciderCancelledException;

    /**
     * Cancel the inferior and have it call cancel on
     * the set of inferiors specified.
     *
     * @param String id The identity of the superior that the inferiors are
     *               enlisted with.
     * @param String[] atoms The list of inferiors that are to be cancelled.
     * @param Qualifier[] qualifiers Any additional qualifiers that may affect
     *                    the cancel.
     * @exception InvalidDeciderException Thrown if the decider identity is invalid.
     * @exception InvalidInferiorException Thrown if the inferior list is invalid.
     * @exception WrongStateException Thrown if the superior is in a state that does
     *                       not allow it to issue cancel.
     * @exception HazardException Thrown if the inferiors generate a hazard heuristic.
     * @exception MixedException Thrown if the inferiors generate a mixed heuristic.
     * @exception DeciderConfirmedException Thrown if all inferiors confirm
     * during cancel.
     * @exception SystemException Thrown in the event of a general fault.
     *
     * @return a status value for each inferior.
     */

    public StatusItem[] cancel_inferiors (String id, String[] atoms, Qualifier[] qualifiers) throws SystemException, InvalidInferiorException, WrongStateException, HazardException, MixedException, InvalidDeciderException, DeciderConfirmedException;

    /**
     * Inform the specified inferior that it has generated a contradiction.
     *
     * @param String id The identity of the superior.
     * @param Qualifier[] qualifiers Any additional qualifiers that may affect
     *                    the operation.
     * @exception InvalidSuperior Thrown if the superior identity is invalid.
     * @exception WrongStateException Thrown if the superior is in an invalid state.
     * @exception SystemException Thrown in the event of a general fault.
     */

    public void contradiction (String id, Qualifier[] qualifiers) throws SystemException, InvalidInferiorException, WrongStateException;

    /**
     * @param String id The identity of the superior.
     * @param Qualifier[] qualifiers Any additional qualifiers that may affect
     *                    the operation.
     *
     * @return the current status of the specified inferior.
     */

    public int status (String id, Qualifier[] qualifiers) throws SystemException, InvalidInferiorException;

}
 

 
 
