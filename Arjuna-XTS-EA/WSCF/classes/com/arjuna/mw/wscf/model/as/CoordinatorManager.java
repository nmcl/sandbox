/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: CoordinatorManager.java,v 1.1 2003/01/07 10:33:41 nmcl Exp $
 */

package com.arjuna.mw.wscf.model.as;

import com.arjuna.mw.wscf.common.*;

import com.arjuna.mw.wscf.model.as.coordinator.*;

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
 * @version $Id: CoordinatorManager.java,v 1.1 2003/01/07 10:33:41 nmcl Exp $
 * @since 1.0.
 */

public interface CoordinatorManager
{

    /**
     * Enrol the specified participant with the coordinator associated with
     * the current thread. If the coordinator supports a priority ordering
     * of participants, then that ordering can also be specified. Any
     * qualifiers that are to be associated with the participant are also
     * provided
     *
     * @param Participant act The participant.
     * @param int priority The priority to associate with the participant in
     * the coordinator's list.
     * @param Qualifier[] quals Any qualifiers to be associated with the
     * participant.
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

    public void addParticipant (Participant act, int priority, Qualifier[] quals) throws NoActivityException, WrongStateException, DuplicateParticipantException, InvalidParticipantException, SystemException;

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

    public void removeParticipant (Participant act) throws NoActivityException, InvalidParticipantException, WrongStateException, SystemException;

    /**
     * Some coordination protocol messages may have asynchronous responses
     * or it may be possible for participants to autonomously generate
     * responses to messages that have not yet been producted by the
     * coordinator. As such, this method allows a response from a participant
     * to be passed to the coordinator. In order to ensure that the protocol
     * remains valid, it is necessary for the participant to specify what
     * message produced the response: if the response was autonomously
     * generated by the participant on the assumption it would receive this
     * message from the coordinator and the coordinator subsequently decides
     * not to produce such a message, then the action taken by the participant
     * is invalid and hence so is the response.
     *
     * @param String id the unique participant identification.
     * @param Message notification the message the participant got/assumed
     * when producing the response.
     * @param Outcome response the actual response.
     * @param Qualifier[] quals any qualifiers associated with the response.
     *
     * @exception InvalidParticipantException Thrown if the coordinator has no
     * knowledge of the participant.
     * @exception WrongStateException Thrown if the coordinator is in a state
     * that does not allow it to accept responses at all or this specific
     * type of response.
     * @exception SystemException Thrown if any other error occurs.
     */

    public void setResponse (String id, Message notification, Outcome response, Qualifier[] quals) throws InvalidParticipantException, NoCoordinatorException, WrongStateException, SystemException;
    
}
