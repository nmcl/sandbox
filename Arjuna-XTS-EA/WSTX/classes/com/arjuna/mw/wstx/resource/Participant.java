/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Participant.java,v 1.1 2002/11/25 11:00:52 nmcl Exp $
 */

package com.arjuna.mw.wstx.resource;

import com.arjuna.mw.wscf.common.Qualifier;

import com.arjuna.mw.wstx.common.Vote;

import com.arjuna.mw.wsas.activity.Outcome;

import com.arjuna.mw.wsas.completionstatus.CompletionStatus;

import com.arjuna.mw.wsas.status.Status;

import com.hp.mw.ts.arjuna.state.*;

import com.arjuna.mw.wsas.exceptions.SystemException;
import com.arjuna.mw.wsas.exceptions.WrongStateException;

import com.arjuna.mw.wstx.exceptions.HeuristicHazardException;
import com.arjuna.mw.wstx.exceptions.HeuristicMixedException;
import com.arjuna.mw.wstx.exceptions.HeuristicCommitException;
import com.arjuna.mw.wstx.exceptions.HeuristicRollbackException;
import com.arjuna.mw.wstx.exceptions.InvalidParticipantException;

/**
 * This is the interface that all two-phase aware participants must define.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: Participant.java,v 1.1 2002/11/25 11:00:52 nmcl Exp $
 * @since 1.0.
 */

public interface Participant
{

    /**
     * Prepare the participant.
     *
     * @param Qualifier[] qualifiers Any additional information.
     *
     * @exception InvalidParticipantException Thrown if the participant identity is invalid
     *            (e.g., refers to an unknown participant.)
     * @exception WrongStateException Thrown if the state of the participant is such that
     *            it cannot prepare.
     * @exception HeuristicHazardException Thrown if upon preparing, the participant finds that
     *            some of its enlisted participants have return statuses which
     *            mean it cannot determine what the result of issuing prepare
     *            to them has been.
     * @exception HeuristicMixedException Thrown if upon preparing, the participant finds that
     *            some of its enlisted participants have return statuses which
     *            mean some of them cancelled and some of them confirmed.
     * @exception SystemException Thrown if some other error occurred.
     *
     * @return the vote.
     */
    
    public Vote prepare (Qualifier[] qualifiers) throws InvalidParticipantException, WrongStateException, HeuristicHazardException, HeuristicMixedException, SystemException;

    /**
     * Commit the participant.
     *
     * @param Qualifier[] qualifiers Any additional information.
     *
     * @exception InvalidParticipantException Thrown if the participant identity is invalid
     *            (e.g., refers to an unknown participant.)
     * @exception WrongStateException Thrown if the state of the participant is such that
     *            it cannot confirm.
     * @exception HeuristicHazardException Thrown if upon preparing, the participant finds that
     *            some of its enlisted participants have return statuses which
     *            mean it cannot determine what the result of issuing confirm
     *            to them has been.
     * @exception HeuristicMixedException Thrown if upon preparing, the participant finds that
     *            some of its enlisted participants have return statuses which
     *            mean some of them cancelled and some of them confirmed.
     * @exception HeuristicRollbackException Thrown if the participant rolls
     * back rather than commits.
     * @exception SystemException Thrown if some other error occurred.
     */

    public void commit (Qualifier[] qualifiers) throws InvalidParticipantException, WrongStateException, HeuristicHazardException, HeuristicMixedException, HeuristicRollbackException, SystemException;

    /**
     * Rollback the participant.
     *
     * @param Qualifier[] qualifiers Any additional information.
     *
     * @exception InvalidParticipantException Thrown if the participant identity is invalid
     *            (e.g., refers to an unknown participant.)
     * @exception WrongStateException Thrown if the state of the participant is such that
     *            it cannot cancel.
     * @exception HeuristicHazardException Thrown if upon preparing, the participant finds that
     *            some of its enlisted participants have return statuses which
     *            mean it cannot determine what the result of issuing cancel
     *            to them has been.
     * @exception HeuristicMixedException Thrown if upon preparing, the participant finds that
     *            some of its enlisted participants have return statuses which
     *            mean some of them cancelled and some of them confirmed.
     * @exception HeuristicCommitException Thrown if the participant commits
     * rather than rolls back.
     * @exception SystemException Thrown if some other error occurred.
     */

    public void rollback (Qualifier[] qualifiers) throws InvalidParticipantException, WrongStateException, HeuristicHazardException, HeuristicMixedException, HeuristicCommitException, SystemException;

    /**
     * Commit the participant in a single phase.
     *
     * @param Qualifier[] qualifiers Any additional information.
     *
     * @exception InvalidParticipantException Thrown if the participant identity is invalid
     *            (e.g., refers to an unknown participant.)
     * @exception WrongStateException Thrown if the state of the participant is such that
     *            it cannot cancel.
     * @exception HeuristicHazardException Thrown if upon preparing, the participant finds that
     *            some of its enlisted participants have return statuses which
     *            mean it cannot determine what the result of issuing cancel
     *            to them has been.
     * @exception HeuristicMixedException Thrown if upon preparing, the participant finds that
     *            some of its enlisted participants have return statuses which
     *            mean some of them cancelled and some of them confirmed.
     * @exception HeuristicRollbackException Thrown if the participant rolls
     * back rather than commit.
     * @exception SystemException Thrown if some other error occurred.
     */

    public void commitOnePhase (Qualifier[] qualifiers) throws InvalidParticipantException, WrongStateException, HeuristicHazardException, HeuristicMixedException, HeuristicRollbackException, SystemException;

    /**
     * Inform the participant that is can forget the heuristic result.
     *
     * @param Qualifier[] qualifiers Any additional qualifiers that may affect
     *                    the operation.
     * @exception InvalidParticipantException Thrown if the participant identity is invalid.
     * @exception WrongStateException Thrown if the participant is in an invalid state.
     * @exception SystemException Thrown in the event of a general fault.
     */

    public void forget (Qualifier[] qualifiers) throws InvalidParticipantException, WrongStateException, SystemException;

    /**
     * @param Qualifier[] qualifiers Any additional qualifiers that may affect
     *                    the operation.
     * @exception InvalidParticipantException Thrown if the participant identity is invalid.
     * @exception SystemException Thrown in the event of a general error.
     *
     * @return the current status of the specified participant.
     */

    public Status status (Qualifier[] qualifiers) throws InvalidParticipantException, SystemException;

    /**
     * @return the name of this participant.
     */

    public String name ();
    
    /**
     * These methods are required so that the coordinator can serialise and
     * de-serialise information about the participant during completion and
     * recovery.
     */

    public boolean packState (OutputObjectState os);

    public boolean unpackState (InputObjectState os);
    
}

