package javax.jaxtx.model.btp.opt;

import javax.jaxtx.model.btp.Qualifier;
import javax.jaxtx.model.btp.StatusItem;

import javax.jaxtx.exceptions.WrongStateException;
import javax.jaxtx.exceptions.SystemException;

import javax.jaxtx.model.btp.HazardException;
import javax.jaxtx.model.btp.MixedException;

/**
 * This is the interface that all superiors provide. It enables new
 * atoms or cohesions to be created and participants to be
 * enrolled/resigned from the specific "transaction".
 */

public interface Superior
{

    /**
     * Start a new transaction (atom or cohesion). If the parent parameter
     * is set then the newly created transaction is automatically
     * enrolled with the specified transaction as a participant.
     *
     * @param String parent The identity of any parent transaction. If present,
     *                   the newly created transaction will be nested or
     *                   enrolled with the parent depending upon the nature of
     *                   any qualifiers.
     * @param Qualifier[] qualifiers Any additional qualifiers that may affect
     *                    the begin.
     * @exception InvalidSuperiorException Thrown if the superior identity is invalid.
     * @exception SystemException Thrown in the event of a general fault.
     * @exception WrongStateException Thrown if the superior is in an invalid state.
     *
     * @return the identity of the newly created transaction.
     */ 

    public String begin (String parent, Qualifier[] qualifiers) throws SystemException, InvalidSuperiorException, WrongStateException;

    /**
     * The specified inferior (identity and inferior handle) is enlisted with
     * the superior.
     *
     * @param String id The identity of the superior to enrol with.
     * @param String participantId The identity of the inferior to be enrolled.
     *            this may be null and in which case a new identity will
     *            be created.
     * @param Inferior participant The inferior to be enrolled.
     * @param Qualifier[] qualifiers Any additional qualifiers that may affect
     *                    the enrol.
     * @exception InvalidSuperiorException Thrown if the superior identity is invalid.
     * @exception WrongStateException Thrown if the superior is in an invalid state.
     * @exception DuplicateInferiorException Thrown if an inferior with the specified
     * id has already been enrolled.
     * @exception SystemException Thrown in the event of a general fault.
     */

    public void enrol (String id, String participantId, Inferior participant, Qualifier[] qualifiers) throws SystemException, InvalidSuperiorException, WrongStateException, DuplicateInferiorException;

    /**
     * The specified inferior resigns from the inferior.
     *
     * @param String id The identity of the superior to resign from.
     * @param String participantId The identity of the inferior to resign.
     * @param Qualifier[] qualifiers Any additional qualifiers that may affect
     *                    the resign.
     * @exception InvalidSuperiorException Thrown if the superior identity is invalid.
     * @exception InvalidInferiorException Thrown if the inferior is unknown to this superior (e.g., it has not been enrolled.)
     * @exception WrongStateException Thrown if the superior is in an invalid state.
     * @exception SystemException Thrown in the event of a general fault.
     */

    public void resign (String id, String participantId, Qualifier[] qualifiers) throws SystemException, InvalidSuperiorException, InvalidInferiorException, WrongStateException;

    /**
     * @param String id The identity of the superior to enquire of.
     * @param String participants The list of inferiors whose status we require.
     * @exception InvalidSuperiorException Thrown if the superior identity is invalid.
     * @exception SystemException Thrown in the event of a general fault.
     *
     * @return the status for the specified inferiors.
     * @see javax.jaxtx.models.btp.StatusItem
     */

    public StatusItem[] statuses (String id, String[] participants) throws SystemException, InvalidSuperiorException;

    /**
     * A participant may unilaterally decide that it can prepare and it
     * must inform the coordinator.
     *
     * @param String id The identity of the superior to inform.
     * @param String participantId The participant identifier.
     * @param Qualifier[] qualifiers Any additional qualifiers that may affect
     *                    the prepare call.
     * @exception InvalidSuperiorException Thrown if the superior identity is invalid.
     * @exception InvalidInferiorException Thrown if the inferior is unknown to this superior (e.g., it has not been enrolled.)
     * @exception WrongStateException Thrown if the superior is in an invalid state.
     * @exception SystemException Thrown in the event of a general fault.
     */

    public void prepared (String id, String participantId, Qualifier[] qualifiers) throws InvalidSuperiorException, InvalidInferiorException, WrongStateException, SystemException;

    /**
     * A participant may unilaterally decide to confirm after it has
     * prepared.
     *
     * @param String id The identity of the superior to inform.
     * @param String participantId The participant identifier.
     * @param Qualifier[] qualifiers Any additional qualifiers that may affect
     *                    the confirmed call.
     * @exception InvalidSuperiorException Thrown if the superior identity is invalid.
     * @exception InvalidInferiorException Thrown if the inferior is unknown to this superior (e.g., it has not been enrolled.)
     * @exception WrongStateException Thrown if the superior is in an invalid state.
     * @exception SystemException Thrown in the event of a general fault.
     */

    public void confirmed (String id, String participantId, Qualifier[] qualifiers) throws InvalidSuperiorException, InvalidInferiorException, WrongStateException, SystemException;

    /**
     * A participant may unilaterally decide to cancel at any time, i.e.,
     * even if it has not been prepared.
     *
     * @param String id The identity of the superior to inform.
     * @param String participantId The participant identifier.
     * @param Qualifier[] qualifiers Any additional qualifiers that may affect
     *                    the cancelled call.
     * @exception InvalidSuperiorException Thrown if the superior identity is invalid.
     * @exception InvalidInferiorException Thrown if the inferior is unknown to this superior (e.g., it has not been enrolled.)
     * @exception WrongStateException Thrown if the superior is in an invalid state.
     * @exception SystemException Thrown in the event of a general fault.
     */

    public void cancelled (String id, String participantId, Qualifier[] qualifiers) throws InvalidSuperiorException, InvalidInferiorException, WrongStateException, SystemException;

}
