/*
 * Copyright (C) 2003,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: UserTransaction.java,v 1.6 2003/04/04 15:00:54 nmcl Exp $
 */

package com.arjuna.mw.wst;

import com.arjuna.wst.*;

/**
 * This is the interface that allows transactions to be started and terminated.
 * The messaging layer converts the Commit, Rollback and Notify messages into
 * calls on this.
 * Importantly, a UserTransaction does not represent a specific transaction,
 * but rather is responsible for providing access to an implicit per-thread 
 * transaction context; it is similar to the UserTransaction in the JTA
 * specification. Therefore, all of the UserTransaction methods implicitly act
 * on the current thread of control.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: UserTransaction.java,v 1.6 2003/04/04 15:00:54 nmcl Exp $
 * @since XTS 1.0.
 */

public interface UserTransaction
{
    
    /**
     * Start a new transaction. If one is already associated with this thread
     * then the WrongStateException will be thrown. Upon success, this
     * operation associates the newly created transaction with the current
     * thread.
     */

    public void begin () throws WrongStateException, SystemException;

    /**
     * Start a new transaction with the specified timeout as its lifetime.
     * If one is already associated with this thread then the
     * WrongStateException will be thrown.
     */

    public void begin (int timeout) throws WrongStateException, SystemException;
    
    /**
     * The transaction is committed by the commit method. This will execute
     * the PhaseZero, 2PC and OutcomeNotification protocols prior to returning.
     * If there is no transaction associated with the invoking thread then
     * UnknownTransactionException is thrown. If the transaction ultimately
     * rolls back then the TransactionRolledBackException is thrown. When
     * complete, this operation disassociates the transaction from the current
     * thread such that it becomes associated with no transaction.
     */

    public void commit () throws TransactionRolledBackException, UnknownTransactionException, SystemException;

    /**
     * The transaction is committed by the commit method. This will execute
     * the PhaseZero, 2PC and OutcomeNotification protocols prior to returning.
     * If there is no transaction associated with the invoking thread then
     * UnknownTransactionException is thrown. If the transaction ultimately
     * rolls back then the TransactionRolledBackException is thrown. When
     * complete, this operation disassociates the transaction from the current
     * thread such that it becomes associated with no transaction.
     *
     * Explicitly acknowledge receipt of the transaction outcome.
     */

    public void commit (boolean withAck) throws TransactionRolledBackException, UnknownTransactionException, SystemException;
    
    /**
     * The rollback operation will terminate the transaction and return
     * normally if it succeeded, while throwing an appropriate exception if it
     * didn't. If there is no transaction associated with the invoking thread
     * then UnknownTransactionException is thrown.
     */

    public void rollback () throws UnknownTransactionException, SystemException;

    /**
     * The rollback operation will terminate the transaction and return
     * normally if it succeeded, while throwing an appropriate exception if it
     * didn't. If there is no transaction associated with the invoking thread
     * then UnknownTransactionException is thrown.
     *
     * Explicitly acknowledge receipt of the transaction outcome.
     */

    public void rollback (boolean withAck) throws UnknownTransactionException, SystemException;

    /**
     * Inform the transaction coordinator that it can release information
     * on the specified transaction. This is the end part of the
     * CompletionWithAck protocol and signifies to the coordinator that the
     * client has received the transation result.
     */

    public void release () throws UnknownTransactionException, SystemException;

    public String transactionIdentifier ();
    
    public String toString ();
    
}
