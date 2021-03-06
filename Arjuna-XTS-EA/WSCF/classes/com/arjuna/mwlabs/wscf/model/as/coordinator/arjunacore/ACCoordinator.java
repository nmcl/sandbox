/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ACCoordinator.java,v 1.1 2003/01/07 10:33:52 nmcl Exp $
 */

package com.arjuna.mwlabs.wscf.model.as.coordinator.arjunacore;

import com.arjuna.mw.wsas.context.Context;

import com.hp.mw.ts.arjuna.common.Uid;

import com.hp.mw.ts.arjuna.coordinator.*;

import com.arjuna.mw.wscf.common.Qualifier;
import com.arjuna.mw.wscf.common.CoordinatorId;

import com.arjuna.mw.wscf.model.as.coordinator.twophase.common.*;
import com.arjuna.mw.wscf.model.as.coordinator.twophase.outcomes.*;

import com.arjuna.mw.wscf.model.as.coordinator.Participant;
import com.arjuna.mw.wscf.model.as.coordinator.Coordinator;
import com.arjuna.mw.wscf.model.as.coordinator.Message;

import com.arjuna.mw.wsas.UserActivityFactory;
import com.arjuna.mw.wsas.UserActivity;

import com.arjuna.mw.wsas.activity.ActivityHierarchy;
import com.arjuna.mw.wsas.activity.ActivityHandle;
import com.arjuna.mw.wsas.activity.Outcome;
import com.arjuna.mw.wsas.activity.HLS;

import com.arjuna.mw.wsas.completionstatus.CompletionStatus;
import com.arjuna.mw.wsas.completionstatus.Success;

import com.arjuna.mwlabs.wsas.context.soap.SOAPContextImple;

import com.arjuna.mw.wsas.exceptions.SystemException;
import com.arjuna.mw.wsas.exceptions.WrongStateException;
import com.arjuna.mw.wsas.exceptions.ProtocolViolationException;

import com.arjuna.mw.wscf.exceptions.*;

import java.util.Hashtable;

/**
 * This class represents a specific coordination instance. It is essentially
 * an ArjunaCore TwoPhaseCoordinator, which gives us access to two-phase with
 * synchronization support but without thread management.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: ACCoordinator.java,v 1.1 2003/01/07 10:33:52 nmcl Exp $
 * @since 1.0.
 */

public class ACCoordinator extends TwoPhaseCoordinator implements com.arjuna.mw.wscf.model.as.coordinator.Coordinator
{

    public ACCoordinator ()
    {
	super();
	
	_theId = new CoordinatorIdImple(get_uid());
    }

    public ACCoordinator (Uid recovery)
    {
	super(recovery);

	_theId = new CoordinatorIdImple(get_uid());
    }
    
    /**
     * If the application requires and if the coordination protocol supports
     * it, then this method can be used to execute a coordination protocol on
     * the currently enlisted participants at any time prior to the termination
     * of the coordination scope.
     *
     * This implementation only supports coordination at the end of the
     * activity.
     *
     * @param CompletionStatus cs The completion status to use when determining
     * how to execute the protocol.
     *
     * @exception WrongStateException Thrown if the coordinator is in a state
     * the does not allow coordination to occur.
     * @exception ProtocolViolationException Thrown if the protocol is violated
     * in some manner during execution.
     * @exception SystemException Thrown if any other error occurs.
     *
     * @return The result of executing the protocol, or null.
     */

    public Outcome coordinate (CompletionStatus cs) throws WrongStateException, ProtocolViolationException, SystemException
    {
	return null;
    }

    /**
     * Enrol the specified participant with the coordinator associated with
     * the current thread. If the coordinator supports a priority ordering
     * of participants, then that ordering can also be specified. Any
     * qualifiers that are to be associated with the participant are also
     * provided.
     *
     * @param Participant act The participant.
     * @param int priority The priority to associate with the participant in
     * the coordinator's list. The two-phase coordinator only supports two
     * types of priority, defined in the Priorities class: Synchronizations
     * and Participants.
     * @param Qualifier[] quals Any qualifiers to be associated with the
     * participant.
     *
     * @exception WrongStateException Thrown if the coordinator is not in a
     * state that allows participants to be enrolled.
     * @exception DuplicateParticipantException Thrown if the participant has
     * already been enrolled and the coordination protocol does not support
     * multiple entries.
     * @exception InvalidParticipantException Thrown if the participant is invalid.
     * @exception SystemException Thrown if any other error occurs.
     *
     * @see com.arjuna.mw.wscf.model.as.coordinator.twophase.common.Priorities
     */

    public void addParticipant (Participant act, int priority, Qualifier[] quals) throws WrongStateException, DuplicateParticipantException, InvalidParticipantException, SystemException
    {
	if (act == null)
	    throw new InvalidParticipantException();

	if (priority == Priorities.SYNCHRONIZATION)
	{
	    SynchronizationRecord rec = new SynchronizationRecord(act, new Uid(), priority, quals);

	    if (addSynchronization(rec) != AddOutcome.AR_ADDED)
		throw new WrongStateException();
	}
	else
	{
	    AbstractRecord rec = new ParticipantRecord(act, new Uid(), priority, quals);

	    if (add(rec) != AddOutcome.AR_ADDED)
		throw new WrongStateException();
	}
    }

    /**
     * Remove the specified participant from the coordinator's list.
     * This operation may not be supported by all coordination protocols.
     *
     * @exception InvalidParticipantException Thrown if the participant is not known
     * of by the coordinator.
     * @exception WrongStateException Thrown if the state of the coordinator
     * does not allow the participant to be removed (e.g., in a two-phase
     * protocol the coordinator is committing.)
     * @exception SystemException Thrown if any other error occurs.
     */
    
    public void removeParticipant (Participant act) throws InvalidParticipantException, WrongStateException, SystemException
    {
	if (act == null)
	    throw new InvalidParticipantException();
	else
	    throw new WrongStateException("ArjunaCore does not support removeParticipant");
    }

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
    
    public void setResponse (String id, Message notification, Outcome response, Qualifier[] quals) throws InvalidParticipantException, WrongStateException, SystemException
    {
	Uid participant = new Uid(id);
	
	if (!participant.valid())
	    throw new InvalidParticipantException("Invalid id: "+id);
	
	synchronized (this)
	{
	    if ((status() == ActionStatus.COMMITTED) ||
		(status() == ActionStatus.ABORTED))
	    {
		throw new WrongStateException();
	    }

	    boolean found = false;
	    RecordList intentionsList = pendingList;  // what about other lists?
	    AbstractRecord rec = intentionsList.peekFront();

	    while (rec != null)
	    {
		if (rec.order().equals(participant))
		{
		    found = true;
		    
		    break;
		}
		else
		    rec = intentionsList.peekNext(rec);
	    }
	    
	    if (!found)
		throw new InvalidParticipantException("Unknown participant: "+id);
	    else
	    {
		if (rec instanceof com.arjuna.mwlabs.wscf.model.as.coordinator.arjunacore.ParticipantRecord)
		{
		    com.arjuna.mwlabs.wscf.model.as.coordinator.arjunacore.ParticipantRecord theRecord = (com.arjuna.mwlabs.wscf.model.as.coordinator.arjunacore.ParticipantRecord) rec;
		    
		    theRecord.setResponse(notification, response);
		}
		else
		    throw new InvalidParticipantException("Participant is not of correct type!");
	    }
	}
    }

    /**
     * @exception SystemException Thrown if any error occurs.
     *
     * @return a reference to the current coordinators' parent if it is nested,
     * null otherwise.
     */
    
    public Coordinator getParentCoordinator () throws SystemException
    {
	return (Coordinator) parent();
    }

    /**
     * @exception SystemException Thrown if any error occurs.
     *
     * @return the complete list of qualifiers that have been registered with
     * the current coordinator.
     */

    public Qualifier[] qualifiers () throws SystemException
    {
	return null;
    }

    /**
     * @exception SystemException Thrown if any error occurs.
     *
     * @return The unique identity of the current coordinator.
     */

    public CoordinatorId identifier () throws SystemException
    {
	return _theId;
    }

    private CoordinatorIdImple _theId;
    
}
