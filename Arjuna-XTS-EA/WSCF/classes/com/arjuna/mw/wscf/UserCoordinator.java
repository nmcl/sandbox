/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: UserCoordinator.java,v 1.1 2002/11/25 10:56:31 nmcl Exp $
 */

package com.arjuna.mw.wscf;

import com.arjuna.mw.wscf.common.CoordinatorId;
import com.arjuna.mw.wscf.common.Qualifier;

import com.arjuna.mw.wsas.UserActivity;

import com.arjuna.mw.wsas.activity.Outcome;

import com.arjuna.mw.wsas.completionstatus.CompletionStatus;

import com.arjuna.mw.wsas.status.Status;

import com.arjuna.mw.wsas.exceptions.WrongStateException;
import com.arjuna.mw.wsas.exceptions.SystemException;
import com.arjuna.mw.wsas.exceptions.InvalidTimeoutException;
import com.arjuna.mw.wsas.exceptions.ProtocolViolationException;
import com.arjuna.mw.wsas.exceptions.NoActivityException;
import com.arjuna.mw.wsas.exceptions.NoPermissionException;

/**
 * The user portion of the coordinator API. An implementation of this interface
 * presents each thread with the capability to create and manage coordinators.
 * It is very similar to the OTS Current and JTA UserTransaction.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: UserCoordinator.java,v 1.1 2002/11/25 10:56:31 nmcl Exp $
 * @since 1.0.
 */

public interface UserCoordinator extends UserActivity
{

    /**
     * Coordinate the participants using the current completion status
     * associated with the activity. This does not necessarily complete the
     * coordination domain, i.e., it may be possible to call this method
     * multiple times within the lifetime of a single activity.
     * 
     * @exception WrongStateException Thrown if the current activity is not in a
     * state that allows it to be coordinated, or is incompatible with the
     * completion status provided.
     * @exception ProtocolViolationException Thrown if the a violation of the
     * activity service or HLS protocol occurs.
     * @exception NoActivityException Thrown if there is no activity
     * associated with the invoking thread.
     * @exception SystemException Thrown if some other error occurred.
     *
     * @return the result of completing the activity. Null is valid and must
     * be interpreted within the context of any HLS that may exist.
     *
     * @see com.arjuna.mw.wsas.activity.Outcome
     */

    public Outcome coordinate () throws WrongStateException, ProtocolViolationException, NoActivityException, SystemException;


    /**
     * Coordinate the participants using the completion status provided.
     * This does not necessarily complete the coordination domain, i.e., it
     * may be possible to call this method multiple times within the lifetime
     * of a single activity.
     * 
     * @exception WrongStateException Thrown if the current activity is not in a
     * state that allows it to be coordinated, or is incompatible with the
     * completion status provided.
     * @exception ProtocolViolationException Thrown if the a violation of the
     * activity service or HLS protocol occurs.
     * @exception NoActivityException Thrown if there is no activity
     * associated with the invoking thread.
     * @exception SystemException Thrown if some other error occurred.
     *
     * @param CompletionStatus cs The CompletionStatus to use.
     *
     * @return the result of completing the activity. Null is valid and must
     * be interpreted within the context of any HLS that may exist.
     *
     * @see com.arjuna.mw.wsas.activity.Outcome
     */

    public Outcome coordinate (CompletionStatus cs) throws WrongStateException, ProtocolViolationException, NoActivityException, SystemException;

    /**
     * Each participant can supply qualifiers to the coordinator at the time
     * it enrols. These qualifiers may be used within the coordination protocol
     * and hence need to be made available to users.
     *
     * @exception NoActivityException Thrown if there is no activity
     * associated with the invoking thread.
     * @exception SystemException Thrown if some other error occurred.
     *
     * @return Qualifier[] qualifiers The qualifiers for all of the enlisted
     * participants, or null if there are none.
     */

    public Qualifier[] qualifiers () throws NoActivityException, SystemException;

    /**
     * @exception NoActivityException Thrown if there is no activity
     * associated with the invoking thread.
     * @exception SystemException Thrown if some other error occurred.
     *
     * @return the unique coordinator id for the current coordinator. This
     * may or may not be the same as the activity id.
     */

    public CoordinatorId identifier () throws NoActivityException, SystemException;
    
}

