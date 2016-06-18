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

    public void enlistForPhaseZero (PhaseZeroParticipant pzp) throws WrongStateException, UnknownTransactionException, AlreadyRegisteredException, SystemException; // no transaction!

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

    public void enlistForTwoPhase (TwoPCParticipant tpp) throws WrongStateException, UnknownTransactionException, AlreadyRegisteredException, SystemException;

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

    public void enlistForOutcomeNotification (OutcomeNotificationParticipant onp) throws WrongStateException, UnknownTransactionException, AlreadyRegisteredException, SystemException;

    public int replay () throws SystemException;

}
