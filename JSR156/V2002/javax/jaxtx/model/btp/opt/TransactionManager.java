package javax.jaxtx.model.btp.opt;

import javax.jaxtx.UserTransaction;

import javax.jaxtx.exceptions.SystemException;

/**
 * This is the interface that application servers/web services will use
 * to communicate with the underlying transaction system. It provides access
 * to all of the routines that a client will use (e.g., begin and end), and
 * also gives access to the Transaction class which allows services to
 * enlist/delist participants.
 */

public interface TransactionManager
{

    /**
     * Return the transaction associated with the invoking thread.
     *
     * @exception SystemException Thrown if an error occurs.
     * @return The transaction associated with the invoking thread.
     * @see javax.jaxtx.Transaction
     */

    public Transaction getTransaction () throws SystemException;

}
