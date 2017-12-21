/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: CoordinatorManager.java,v 1.1 2003/01/07 10:33:44 nmcl Exp $
 */

package com.arjuna.mw.wscf.model.twophase.api;

import com.arjuna.mw.wscf.common.*;

import com.arjuna.mw.wscf.model.twophase.participants.*;
import com.arjuna.mw.wscf.model.twophase.exceptions.*;

import com.arjuna.mw.wsas.activity.Outcome;

import com.arjuna.mw.wscf.exceptions.*;

import com.arjuna.mw.wsas.exceptions.WrongStateException;
import com.arjuna.mw.wsas.exceptions.SystemException;
import com.arjuna.mw.wsas.exceptions.NoActivityException;

/**
 * The CoordinatorManager is the way in which services can enlist
 * participants with the current coordinator.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: CoordinatorManager.java,v 1.1 2003/01/07 10:33:44 nmcl Exp $
 * @since 1.0.
 */

public interface CoordinatorManager extends UserCoordinator
{

    /**
     * Enrol the specified participant with the coordinator associated with
     * the current thread.
     *
     * @param Participant act The participant.
     *
     * @exception NoActivityException Thrown if there is no activity associated
     * with the current thread.
     * @exception WrongStateException Thrown if the coordinator is not in a
     * state that allows participants to be enrolled.
     * @exception DuplicateParticipantException Thrown if the participant has
     * already been enrolled and the coordination protocol does not support
     * multiple entries.
     * @exception InvalidParticipantException Thrown if the participant is invalid.
     * @exception SystemException Thrown if any other error occurs.
     */

    public void enlistParticipant (Participant act) throws NoActivityException, WrongStateException, DuplicateParticipantException, InvalidParticipantException, SystemException;

    /**
     * Remove the specified participant from the coordinator associated with
     * the current thread.
     *
     * @param Participant act The participant to remove.
     *
     * @exception NoActivityException Thrown if there is no activity associated
     * with the current thread.
     * @exception WrongStateException Thrown if the coordinator is not in a
     * state that allows participants to be removed.
     * @exception InvalidParticipantException Thrown if the participant is invalid.
     * @exception SystemException Thrown if any other error occurs.
     */

    public void delistParticipant (Participant act) throws NoActivityException, InvalidParticipantException, WrongStateException, SystemException;

    /**
     * Enrol the specified synchronization with the coordinator associated with
     * the current thread.
     *
     * @param Synchronization act The synchronization to add.
     *
     * @exception NoActivityException Thrown if there is no activity associated
     * with the current thread.
     * @exception WrongStateException Thrown if the coordinator is not in a
     * state that allows participants to be enrolled.
     * @exception DuplicateSynchronizationException Thrown if the participant has
     * already been enrolled and the coordination protocol does not support
     * multiple entries.
     * @exception InvalidSynchronizationException Thrown if the participant is invalid.
     * @exception SystemException Thrown if any other error occurs.
     */

    public void enlistSynchronization (Synchronization act) throws NoActivityException, WrongStateException, DuplicateSynchronizationException, InvalidSynchronizationException, SystemException;

    /**
     * Remove the specified synchronization from the coordinator associated
     * with the current thread.
     *
     * @param Synchronization act The synchronization to remove.
     *
     * @exception NoActivityException Thrown if there is no activity associated
     * with the current thread.
     * @exception WrongStateException Thrown if the coordinator is not in a
     * state that allows participants to be removed.
     * @exception InvalidSynchronizationException Thrown if the participant is invalid.
     * @exception SystemException Thrown if any other error occurs.
     */

    public void delistSynchronization (Synchronization act) throws NoActivityException, InvalidSynchronizationException, WrongStateException, SystemException;

}

