/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: TwoPhaseTxManager.java,v 1.2 2003/03/04 12:59:29 nmcl Exp $
 */

package com.arjuna.mwlabs.wstx.model.as.twophase;

import com.arjuna.mw.wscf.model.as.CoordinatorManager;
import com.arjuna.mw.wscf.model.as.CoordinatorManagerFactory;

import com.arjuna.mw.wscf.model.as.coordinator.twophase.common.*;

import com.arjuna.mw.wstx.resource.Participant;
import com.arjuna.mw.wstx.resource.Synchronization;

import com.arjuna.mw.wstx.TransactionManager;

import com.arjuna.mwlabs.wstx.model.as.twophase.resource.ParticipantAction;
import com.arjuna.mwlabs.wstx.model.as.twophase.resource.SynchronizationAction;

import com.arjuna.mw.wstx.exceptions.DuplicateParticipantException;
import com.arjuna.mw.wstx.exceptions.NoTransactionException;
import com.arjuna.mw.wstx.exceptions.InvalidTransactionException;
import com.arjuna.mw.wstx.exceptions.InvalidParticipantException;
import com.arjuna.mw.wstx.exceptions.DuplicateSynchronizationException;
import com.arjuna.mw.wstx.exceptions.InvalidSynchronizationException;

import com.arjuna.mw.wsas.exceptions.SystemException;
import com.arjuna.mw.wsas.exceptions.WrongStateException;
import com.arjuna.mw.wsas.exceptions.NoActivityException;
import com.arjuna.mw.wsas.exceptions.InvalidActivityException;
import com.arjuna.mw.wsas.exceptions.ActiveChildException;

/**
 * This is the service side component of the user interface. Since services
 * or some entity acting on their behalf (e.g., an interceptor) must enlist
 * participants with the transaction (atom or cohesion), this interface
 * primarily concentrates on those methods. However, a service may well
 * not be the final destination for a transaction, i.e., in order to perform
 * its work, the service may have to make other remote invocations on other
 * services and propagate the context, suspend it, etc. As such, all of the
 * UserTransaction methods are also available to the service.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: TwoPhaseTxManager.java,v 1.2 2003/03/04 12:59:29 nmcl Exp $
 * @since 1.0.
 */

public class TwoPhaseTxManager implements TransactionManager
{

    public TwoPhaseTxManager ()
    {
	try
	{
	    _coordinatorManager = CoordinatorManagerFactory.coordinatorManager();
	}
	catch (Exception ex)
	{
	    // TODO

	    ex.printStackTrace();
	    
	    _coordinatorManager = null;
	}
    }
    
    /**
     * Enrol the specified participant in the current transaction.
     * It is illegal to call this method when no transaction is associated
     * with the thread.
     *
     * @param Participant participant The participant to enrol.
     * @exception WrongStateException Thrown if the transaction is not in a state
     * whereby participants can be enrolled.
     * @exception DuplicateParticipantException Thrown if the participant identifier
     * has already been associated with a participant.
     * @exception NoTransactionException Thrown if there is no transaction
     * associated with the invoking thread.
     * @exception InvalidTransactionException Thrown if the transaction associated with
     * the thread is invalid.
     * @exception InvalidParticipantException Thrown if the participant reference
     * is invalid.
     * @exception SystemException Thrown if any other error occurs.
     */

    public void enlist (Participant participant) throws WrongStateException, DuplicateParticipantException, NoTransactionException, InvalidTransactionException, InvalidParticipantException, SystemException
    {
	try
	{
	    _coordinatorManager.addParticipant(new ParticipantAction(participant), Priorities.PARTICIPANT, null);
	}
	catch (com.arjuna.mw.wscf.exceptions.DuplicateParticipantException ex)
	{
	    throw new DuplicateParticipantException();
	}
	catch (NoActivityException ex)
	{
	    throw new NoTransactionException();
	}
	catch (com.arjuna.mw.wscf.exceptions.InvalidParticipantException ex)
	{
	    throw new InvalidParticipantException();
	}
    }
    
    /**
     * Cause the specified participant to resign from the transaction.
     * CAUTION: use with *extreme* care since if invoked at the wrong time
     * it could lead to data corruption or state modifications that are
     * no longer under the control of the transaction. You *must* ensure
     * that no state changes have been made that should be controlled by
     * the specified participant.
     *
     * @param Participant participant The participant to resign.
     * @exception InvalidTransactionException Thrown if the transaction associated with
     * the thread is invalid.
     * @exception NoTransactionException Thrown if no transaction is associated
     * with the invoking thread.
     * @exception InvalidParticipantException Thrown if the transaction does not know
     * about the specified participant or the parameter is invalid.
     * @exception WrongStateException Thrown if the transaction is not in a state
     * that allows participants to resign.
     * @exception SystemException Thrown if any other error occurs.
     */

    public void delist (Participant participant) throws InvalidTransactionException, NoTransactionException, InvalidParticipantException, WrongStateException, SystemException
    {
	// TODO: support it!

	throw new WrongStateException();
    }

    /**
     * Enlist a synchronization with the current transaction. Synchronizations
     * do not receive the two-phase commit messages but instead are invoked
     * prior to its start and after it has completed.
     *
     * @param Synchronization participant The synchronization to enroll.
     *
     * @exception WrongStateException Thrown if the transaction state is such
     * that synchronizations cannot be enrolled.
     * @exception NoTransactionException Thrown if there is no transaction
     * associated with the invoking thread.
     * @exception InvalidTransactionException Thrown if the transaction is not
     * top-level.
     * @exception InvalidSynchronizationException Thrown if the synchronization
     * reference is invalid.
     * @exception DuplicateSynchronizationException Thrown if the synchronization
     * has already been registered with the transaction.
     * @exception SystemException Thrown if any other error occurs.
     */

    public void addSynchronization (Synchronization participant) throws WrongStateException, NoTransactionException, InvalidTransactionException, InvalidSynchronizationException, DuplicateSynchronizationException, SystemException
    {
	try
	{
	    _coordinatorManager.addParticipant(new SynchronizationAction(participant), Priorities.SYNCHRONIZATION, null);
	}
	catch (com.arjuna.mw.wscf.exceptions.DuplicateParticipantException ex)
	{
	    throw new DuplicateSynchronizationException();
	}
	catch (NoActivityException ex)
	{
	    throw new NoTransactionException();
	}
	catch (com.arjuna.mw.wscf.exceptions.InvalidParticipantException ex)
	{
	    throw new InvalidSynchronizationException();
	}
    }

    /**
     * Remove the specified synchronization participant from the transaction.
     *
     * @param Synchronization participant The participant to remove.
     *
     * @exception InvalidTransactionException Thrown if the transaction is not
     * top-level.
     * @exception NoTransactionException Thrown if there is no transaction
     * associated with the current thread.
     * @exception InvalidSynchronizationException Thrown if the transaction
     * does not know about the specified synchronization.
     * @exception WrongStateException Thrown if the state of the transaction is
     * such that the synchronization cannot be removed.
     * @exception SystemException Thrown if any other error occurs.
     */

    public void removeSynchronization (Synchronization participant) throws SystemException, InvalidTransactionException, NoTransactionException, InvalidSynchronizationException, WrongStateException
    {
	throw new WrongStateException();
    }

    private CoordinatorManager _coordinatorManager;
    
}
