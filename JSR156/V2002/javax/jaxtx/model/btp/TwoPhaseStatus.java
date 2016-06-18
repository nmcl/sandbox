package javax.jaxtx.models.btp;

/**
 * Each participant and coordinator goes through a series of
 * states. These states are represented as values within this class.
 * In the following descriptions, the term <italic>entity</italic>
 * is used to refer to either an inferior or superior.
 * 
 * Note, this class represents the super-set of all possible states
 * an entity may go through. So, a given method invocation may only
 * ever expect to see a subset of these values.
 */

public class TwoPhaseStatus
{
    /**
     * The entity is in the active state, i.e., it has not begun
     * to terminate, either autonomously or by direct coordinator
     * control.
     */

    public static final int ACTIVE = 0;

    /**
     * The inferior has resigned from the transaction as its
     * work has had no effect on the outcome.
     */

    public static final int RESIGNED = 1;

    /**
     * The entity is preparing.
     */

    public static final int PREPARING = 2;

    /**
     * The entity has prepared successfully.
     */

    public static final int PREPARED = 3;

    /**
     * The inferior has autonomously decided to confirm the work
     * that it was to control. This may lead to a heuristic outcome
     * for the transaction if the coordinator reached a different
     * decision.
     */

    public static final int AUTONOMOUSLY_CONFIRMED = 4;

    /**
     * The inferior has autonomously decided to cancel the work
     * that it was to control. This may lead to a heuristic outcome
     * for the transaction if the coordinator reached a different
     * decision.
     */

    public static final int AUTONOMOUSLY_CANCELLED = 5;

    /**
     * The entity is confirming.
     */

    public static final int CONFIRMING = 6;

    /**
     * The entity has successfully confirmed.
     */

    public static final int CONFIRMED = 7;

    /**
     * The entity is cancelling.
     */

    public static final int CANCELLING = 8;

    /**
     * The entity has successfully cancelled.
     */

    public static final int CANCELLED = 9;

    /**
     * Confirm was requested but cancel was received.
     */

    public static final int CANCEL_CONTRADICTION = 10;

    /**
     * Cancel was requested but confirm was received.
     */

    public static final int CONFIRM_CONTRADICTION = 11;

    /**
     * A hazard heuristic has occurred. This means that the transaction
     * cannot determine what each participant did in relation to
     * transaction termination.
     */

    public static final int HAZARD = 12;

    /**
     * The entity has been created but has not yet been "started".
     * (Whatever that means for the entity!)
     */

    public static final int CREATED = 13;

    /**
     * The inferior is enrolling with a superior.
     */

    public static final int ENROLLING = 14;

    /**
     * The inferior is resigning from the superior.
     */

    public static final int RESIGNING = 15;

    /**
     * The inferior contradicted the coordinator outcome.
     */

    public static final int CONTRADICTED = 16;

    /**
     * The entity state is unknown. This should be a transient
     * outcome such that retrying will eventually give a definitive
     * status.
     */

    public static final int UNKNOWN = 17;

    /**
     * The status of the entity is currently unobtainable.
     * This should be a transient outcome such that retrying will eventually
     * give a definitive status.
     */

    public static final int INACCESSIBLE = 18;

    /**
     * A mixed heuristic has occurred. This means that the transaction
     * knows that all participants either confirmed or cancelled.
     */

    public static final int MIXED = 19;

    /**
     * There is no transaction with the specified id.
     */

    public static final int INVALID = 20;

    /**
     * There is no transaction.
     */

    public static final int NO_TRANSACTION = 21;
    
    /**
     * @param int status The status value that we want to string-ify.
     * @return a string representation of the specified status value.
     */

    public static String stringForm (int status)
    {
        switch (status)
        {
        case TwoPhaseStatus.ACTIVE:
            return "TwoPhaseStatus.ACTIVE";
        case TwoPhaseStatus.RESIGNED:
            return "TwoPhaseStatus.RESIGNED";
        case TwoPhaseStatus.PREPARING:
            return "TwoPhaseStatus.PREPARING";
        case TwoPhaseStatus.PREPARED:
            return "TwoPhaseStatus.PREPARED";
        case TwoPhaseStatus.AUTONOMOUSLY_CONFIRMED:
            return "TwoPhaseStatus.AUTONOMOUSLY_CONFIRMED";
        case TwoPhaseStatus.AUTONOMOUSLY_CANCELLED:
            return "TwoPhaseStatus.AUTONOMOUSLY_CANCELLED";
        case TwoPhaseStatus.CONFIRMING:
            return "TwoPhaseStatus.CONFIRMING";
        case TwoPhaseStatus.CONFIRMED:
            return "TwoPhaseStatus.CONFIRMED";
        case TwoPhaseStatus.CANCELLING:
            return "TwoPhaseStatus.CANCELLING";
        case TwoPhaseStatus.CANCELLED:
            return "TwoPhaseStatus.CANCELLED";
        case TwoPhaseStatus.CANCEL_CONTRADICTION:
            return "TwoPhaseStatus.CANCEL_CONTRADICTION";
        case TwoPhaseStatus.CONFIRM_CONTRADICTION:
            return "TwoPhaseStatus.CONFIRM_CONTRADICTION";
        case TwoPhaseStatus.HAZARD:
            return "TwoPhaseStatus.HAZARD";
        case TwoPhaseStatus.CREATED:
            return "TwoPhaseStatus.CREATED";
        case TwoPhaseStatus.ENROLLING:
            return "TwoPhaseStatus.ENROLLING";
        case TwoPhaseStatus.RESIGNING:
            return "TwoPhaseStatus.RESIGNING";
        case TwoPhaseStatus.CONTRADICTED:
            return "TwoPhaseStatus.CONTRADICTED";
        case TwoPhaseStatus.INACCESSIBLE:
            return "TwoPhaseStatus.INACCESSIBLE";
        case TwoPhaseStatus.MIXED:
            return "TwoPhaseStatus.MIXED";
        case TwoPhaseStatus.INVALID:
            return "TwoPhaseStatus.INVALID";
        case TwoPhaseStatus.NO_TRANSACTION:
            return "TwoPhaseStatus.NO_TRANSACTION";
        default:
        case TwoPhaseStatus.UNKNOWN:
            return "TwoPhaseStatus.UNKNOWN";
        }
    }
    
}
