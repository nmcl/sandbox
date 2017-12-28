/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ACCoordinator.java,v 1.1 2003/01/07 10:33:53 nmcl Exp $
 */

package com.arjuna.mwlabs.wscf.model.twophase.arjunacore;

import com.arjuna.mw.wsas.context.Context;

import com.hp.mw.ts.arjuna.common.Uid;

import com.hp.mw.ts.arjuna.coordinator.*;

import com.arjuna.mw.wscf.model.twophase.participants.*;
import com.arjuna.mw.wscf.model.twophase.exceptions.*;

import com.arjuna.mw.wscf.common.Qualifier;
import com.arjuna.mw.wscf.common.CoordinatorId;

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
 * @version $Id: ACCoordinator.java,v 1.1 2003/01/07 10:33:53 nmcl Exp $
 * @since 1.0.
 */

public class ACCoordinator extends TwoPhaseCoordinator
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
     * the current thread.
     *
     * @param Participant act The participant.
     *
     * @exception WrongStateException Thrown if the coordinator is not in a
     * state that allows participants to be enrolled.
     * @exception DuplicateParticipantException Thrown if the participant has
     * already been enrolled and the coordination protocol does not support
     * multiple entries.
     * @exception InvalidParticipantException Thrown if the participant is invalid.
     * @exception SystemException Thrown if any other error occurs.
     */

    public void enlistParticipant (Participant act) throws WrongStateException, DuplicateParticipantException, InvalidParticipantException, SystemException
    {
	if (act == null)
	    throw new InvalidParticipantException();

	AbstractRecord rec = new ParticipantRecord(act, new Uid());

	if (add(rec) != AddOutcome.AR_ADDED)
	    throw new WrongStateException();
    }

    /**
     * Remove the specified participant from the coordinator's list.
     *
     * @exception InvalidParticipantException Thrown if the participant is not known
     * of by the coordinator.
     * @exception WrongStateException Thrown if the state of the coordinator
     * does not allow the participant to be removed (e.g., in a two-phase
     * protocol the coordinator is committing.)
     * @exception SystemException Thrown if any other error occurs.
     */
    
    public void delistParticipant (Participant act) throws InvalidParticipantException, WrongStateException, SystemException
    {
	if (act == null)
	    throw new InvalidParticipantException();
	else
	    throw new WrongStateException("ArjunaCore does not support removal of participants");
    }

    /**
     * Enrol the specified synchronization with the coordinator associated with
     * the current thread.
     *
     * @param Synchronization act The synchronization to add.
     *
     * @exception WrongStateException Thrown if the coordinator is not in a
     * state that allows participants to be enrolled.
     * @exception DuplicateSynchronizationException Thrown if the participant has
     * already been enrolled and the coordination protocol does not support
     * multiple entries.
     * @exception InvalidSynchronizationException Thrown if the participant is invalid.
     * @exception SystemException Thrown if any other error occurs.
     */

    public void enlistSynchronization (Synchronization act) throws WrongStateException, DuplicateSynchronizationException, InvalidSynchronizationException, SystemException
    {
	if (act == null)
	    throw new InvalidSynchronizationException();

	SynchronizationRecord rec = new SynchronizationRecord(act, new Uid());

	if (addSynchronization(rec) != AddOutcome.AR_ADDED)
	    throw new WrongStateException();
    }

    /**
     * Remove the specified synchronization from the coordinator's list.
     *
     * @exception InvalidSynchronizationException Thrown if the participant is not known
     * of by the coordinator.
     * @exception WrongStateException Thrown if the state of the coordinator
     * does not allow the participant to be removed (e.g., in a two-phase
     * protocol the coordinator is committing.)
     * @exception SystemException Thrown if any other error occurs.
     */
    
    public void delistSynchronization (Synchronization act) throws InvalidSynchronizationException, WrongStateException, SystemException
    {
	if (act == null)
	    throw new InvalidSynchronizationException();
	else
	    throw new WrongStateException("ArjunaCore does not support removal of synchronizations");
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
