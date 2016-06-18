package javax.jaxtx.completionstatus;

/**
 * The transaction implementations can complete in one of three ways:
 *
 * CONFIRM: the transaction should complete in a success condition. When in
 *          this state the transaction may be moved into any other state.
 * CANCEL: the transaction should cancel its work. What this means will
 *         be implementation specific. When in this state the transaction may
 *         be moved into any other state.
 * CANCEL_ONLY: the transaction should cancel its work. What this means will
 *              be implementation specific. Once in this state, the transaction
 *              cannot be moved into any other state.
 *
 * Additional completion states can be provided by implementations simply
 * by deriving classes/interfaces from this base interface.
 */

public interface CompletionStatus
{
    
}
