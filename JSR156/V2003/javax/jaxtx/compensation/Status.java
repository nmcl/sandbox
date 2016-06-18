package javax.jaxtx.compensation;

/**
 * This enumeration class represents the states that a compensation
 * transaction and participants may be in.
 *
 * @version 1.0
 *
 * $Id: Status.java,v 1.1 2004/06/22 10:07:59 nmcl Exp $
 */

public interface Status
{
    /**
     * The transaction or participant is in an active state. For the
     * transaction this means that it has not started to terminate. In the
     * case of a participant it may mean that the business service is still
     * executing work or that it has not been told to terminate.
     */

    public static final int ACTIVE = 0;

    /**
     * The transaction/participant is in the process of compensating for
     * work performed.
     */

    public static final int COMPENSATING = 1;

    /**
     * The transaction/participant has compensated successfully for
     * work performed.
     */

    public static final int COMPENSATED = 2;

    /**
     * The transaction/participant is in the process of completing. This
     * means that all work performed can be considered done and no further
     * compensation information need be maintained.
     */

    public static final int COMPLETING = 3;

    /**
     * The transaction/participant has completed successfully.
     */

    public static final int COMPLETED = 4;

    /**
     * The transaction/participant is in the process of canceling. This
     * means that all work performed can be considered undone and no further
     * compensation information need be maintained.
     */

    public static final int CANCELING = 5;

    /**
     * The transaction/participant has canceled successfully.
     */

    public static final int CANCELED = 6;

    /**
     * The transaction/participant was asked to compensate, but compensation
     * failed.
     */

    public static final int COMPENSATION_FAILED = 7;

    /**
     * The transaction/participant was asked to complete, but this was not
     * possible.
     */

    public static final int COMPLETION_FAILED = 8;

    /**
     * The only possible outcome of the transaction/participant is for
     * it to cancel or compensate.
     */

    public static final int UNDO_ONLY = 9;

}

