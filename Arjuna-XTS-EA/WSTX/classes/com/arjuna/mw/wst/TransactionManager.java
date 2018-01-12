/*
 * Copyright (C) 2003,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: TransactionManager.java,v 1.8 2003/03/24 10:57:01 nmcl Exp $
 */

package com.arjuna.mw.wst;

import com.arjuna.wst.*;

import com.arjuna.wsc.AlreadyRegisteredException;

/**
 * This is the interface that the core exposes in order to allow different
 * types of participants to be enrolled. The messaging layer continues to
 * work in terms of the registrar, but internally we map to one of these
 * methods.
 *
 * This could also be the interface that high-level users see (e.g., at the
 * application Web Service).
 *
 * As with UserTransaction a TransactionManager does not represent a specific
 * transaction, but rather is responsible for providing access to an implicit
 * per-thread transaction context.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: TransactionManager.java,v 1.8 2003/03/24 10:57:01 nmcl Exp $
 * @since XTS 1.0.
 */

public interface TransactionManager
{

    /**
     * Enlist the specified participant with current transaction such that it
     * will participate in the PhaseZero protocol; a unique identifier for the
     * participant is also required. If there is no transaction associated with
     * the invoking thread then the UnknownTransactionException exception is
     * thrown. If the coordinator already has a participant enrolled with the 
     * same identifier, then AlreadyRegisteredException will be thrown. If the
     * transaction is not in a state where participants can be enrolled (e.g.,
     * it is terminating) then WrongStateException will be thrown.
     */

    public void enlistForPhaseZero (PhaseZeroParticipant pzp, String id) throws WrongStateException, UnknownTransactionException, AlreadyRegisteredException, SystemException;

    /**
     * Enlist the specified participant with current transaction such that it
     * will participate in the 2PC protocol; a unique identifier for the
     * participant is also required. If there is no transaction associated with
     * the invoking thread then the UnknownTransactionException exception is
     * thrown. If the coordinator already has a participant enrolled with the 
     * same identifier, then AlreadyRegisteredException will be thrown. If the
     * transaction is not in a state where participants can be enrolled (e.g.,
     * it is terminating) then WrongStateException will be thrown.
     */

    public void enlistForTwoPhase (TwoPCParticipant tpp, String id) throws WrongStateException, UnknownTransactionException, AlreadyRegisteredException, SystemException;

    /**
     * Enlist the specified participant with current transaction such that it
     * will participate in the OutcomeNotification protocol; a unique
     * identifier for the participant is also required. If there is no
     * transaction associated with the invoking thread then the
     * UnknownTransactionException exception is thrown. If the coordinator
     * already has a participant enrolled with the same identifier, then
     * AlreadyRegisteredException will be thrown. If the transaction is not in 
     * a state where participants can be enrolled (e.g., it is terminating)
     * then WrongStateException will be thrown.
     */

    public void enlistForOutcomeNotification (OutcomeNotificationParticipant onp, String id) throws WrongStateException, UnknownTransactionException, AlreadyRegisteredException, SystemException;
    
    public int replay () throws SystemException;

    /**
     * The resume method can be used to (re-)associate a thread with a 
     * transaction(s) via its TxContext. Prior to association, the thread is
     * disassociated with any transaction(s) with which it may be currently
     * associated. If the TxContext is null, then the thread is associated with
     * no transaction. The UnknownTransactionException exception is thrown if
     * the transaction that the TxContext refers to is invalid in the scope of
     * the invoking thread.
     */

    public void resume (TxContext tx) throws UnknownTransactionException, SystemException;

    /**
     * A thread of control may require periods of non-transactionality so that
     * it may perform work that is not associated with a specific transaction.
     * In order to do this it is necessary to disassociate the thread from any
     * transactions. The suspend method accomplishes this, returning a
     * TxContext instance, which is a handle on the transaction. The thread is
     * then no longer associated with any transaction.
     */

    public TxContext suspend () throws SystemException;
    
    /**
     * The currentTransaction method returns the TxContext for the current
     * transaction, or null if there is none. Unlike suspend, this method does
     * not disassociate the current thread from the transaction(s). This can
     * be used to enable multiple threads to execute within the scope of the
     * same transaction.
     */

    public TxContext currentTransaction () throws SystemException;
    
}
