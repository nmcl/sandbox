package javax.jaxtx.status;

/**
 * This class represents the status of the transaction. Since the status
 * may well be implementation specific, we try to capture the common
 * state changes that transactions go through here.
 *
 * Additional state changes may be represented as classes/interfaces derived
 * from this interface and interpreted in an implementation specific
 * manner.
 *
 * Basic states are:
 *
 * Active: the transaction is active.
 * CompletedCancelStatus: the transaction has completed in a cancel state.
 * CompletedConfirmStatus: the transaction has completed in a confirm state.
 * CompletingCancelStatus: the transaction is in the process of completing
 * in a cancel state.
 * CompletingConfirmStatus: the transaction is in the process of completing
 * in a confirm state.
 * MarkedCancelOnlyStatus: the transaction state has been marked as cancel
 * only, so its only outcome is to eventually cancel.
 * UnknownStatus: the state of the transaction is currently unknown. This
 * should be a transient, so retrying will eventually return one of the
 * other states.
 * NoTransactionStatus: there is no transaction associated with the
 * invoking thread.
 */

public interface Status
{
    
}
