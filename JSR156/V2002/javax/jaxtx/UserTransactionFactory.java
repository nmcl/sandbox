package javax.jaxtx;

import javax.jaxtx.exceptions.SystemException;
import javax.jaxtx.exceptions.InvalidSecurityOptionException;
import javax.jaxtx.exceptions.NoSuchServiceException;

import org.w3c.dom.Document;

/**
 * The UserTransactionFactory is used to obtain a handle on a specific
 * type of transaction model implementation. So, for example, it would
 * be possible to use this to obtain a reference to a UserTransaction that
 * implemented JTA style transactions, or one which implemented BTP
 * type transactions.
 */

public interface UserTransactionFactory
{

    /**
     * Returns parse-able information about the protocol this
     * coordinator implements.
     *
     * @exception SystemException Thrown if any error occurs.
     *
     * @return the XML configuration file, or null if there is not one.
     */

    public org.w3c.dom.Document configuration () throws SystemException;

    /**
     * Get a UserTransaction that corresponds to the transaction type
     * and configuration specified. The factory can use the XML information
     * in any way it deems necessary to find the type of transaction
     * implementation requested. This search algorithm may be specified within
     * the XML document itself.
     * 
     * @param org.w3c.dom.Document conf The configuration information that
     * specifies the type of transaction and other implementation specific
     * data.
     *
     * @exception NoSuchServiceException Thrown if no UserTransaction
     * corresponding to the type required can be found.
     * @exception SystemException Thrown if any other error occurs.
     *
     * @return a handle to a specific transaction implementation.
     */

    public UserTransaction getTransactionType (org.w3c.dom.Document conf) throws NoSuchServiceException, SystemException;

    /**
     * Get all UserTransactions that this factory knows about. Some
     * implementation specific search algorithm may be used.
     *
     * @exception InvalidSecurityOptionException Thrown if the invoking thread
     * is not allowed access to this information.
     * @exception SystemException Thrown if any other error occurs.
     *
     * @return a handle to all transaction implementations.
     */

    public UserTransaction[] getAllTransactionTypes () throws InvalidSecurityOptionException, SystemException;

    /**
     * Get the transaction service implementation that is currently associated
     * with the invoking thread (if any).
     *
     * @exception SystemException Thrown if any error occurs.
     *
     * @return the UserTransaction implementation currently associated with
     * this thread, or null if there is no association.
     */

    public UserTransaction getCurrentTransactionType () throws SystemException;
    
}
