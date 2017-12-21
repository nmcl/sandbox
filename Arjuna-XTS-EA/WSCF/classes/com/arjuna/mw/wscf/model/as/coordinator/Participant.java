/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Participant.java,v 1.1 2003/01/07 10:33:41 nmcl Exp $
 */

package com.arjuna.mw.wscf.model.as.coordinator;

import com.arjuna.mw.wsas.activity.Outcome;

import com.arjuna.mw.wsas.completionstatus.CompletionStatus;

import com.arjuna.mw.wsas.status.Status;

import com.hp.mw.ts.arjuna.state.OutputObjectState;
import com.hp.mw.ts.arjuna.state.InputObjectState;

import com.arjuna.mw.wsas.exceptions.WrongStateException;
import com.arjuna.mw.wsas.exceptions.SystemException;
import com.arjuna.mw.wsas.exceptions.ProtocolViolationException;

/**
 * The Participant represents the participant interface. Because WSCF is a generic
 * coordination framework, the basic participant is obviously generic.
 * Specific participants bound to specific coordination types can be wrapped
 * by this interface, thus hiding it from users.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: Participant.java,v 1.1 2003/01/07 10:33:41 nmcl Exp $
 * @since 1.0.
 */

public interface Participant
{

    /**
     * @exception SystemException Thrown if any error occurs.
     *
     * @return the unique identity for this participant.
     */

    public String identity () throws SystemException;


    /**
     * Process the message from the coordinator and return a response, which
     * may be null - validity is down to the specific coordinator.
     *
     * @param Message notification The message to be processed.
     *
     * @exception WrongStateException Thrown if the participant is not in a
     * state which is valid given the input message.
     * @exception ProtocolViolationException Thrown if the participant has
     * violated the coordination protocol.
     * @exception SystemException Thrown if any other error occurs.
     * 
     * @return the Outcome message representing the result of dealing with
     * the notification. Null may be a valid response.
     */

    public Outcome processMessage (Message notification) throws WrongStateException, ProtocolViolationException, SystemException;

    /*
     * These methods are required so that the coordinator can serialise and
     * de-serialise information about the inferior during completion and
     * recovery.
     */

    /**
     * Pack the state of the participant into the object buffer.
     *
     * @return <code>true</code> if successful, <code>false</code> otherwise.
     */

    public boolean packState (OutputObjectState os);

    /**
     * Unpack the state of the participant from the object buffer.
     *
     * @return <code>true</code> if successful, <code>false</code> otherwise.
     */

    public boolean unpackState (InputObjectState os);
    
}

