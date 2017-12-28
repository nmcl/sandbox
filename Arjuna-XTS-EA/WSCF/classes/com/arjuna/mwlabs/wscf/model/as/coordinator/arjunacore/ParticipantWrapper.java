/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ParticipantWrapper.java,v 1.1 2003/01/07 10:33:52 nmcl Exp $
 */

package com.arjuna.mwlabs.wscf.model.as.coordinator.arjunacore;

import com.hp.mw.ts.arjuna.ObjectType;
import com.hp.mw.ts.arjuna.coordinator.*;
import com.hp.mw.ts.arjuna.common.*;
import com.hp.mw.ts.arjuna.state.*;
import com.hp.mw.ts.arjuna.gandiva.ClassName;

import com.arjuna.mw.wscf.model.as.coordinator.twophase.outcomes.*;
import com.arjuna.mw.wscf.model.as.coordinator.twophase.messages.*;
import com.arjuna.mw.wscf.model.as.coordinator.*;
import com.arjuna.mw.wscf.model.as.coordinator.twophase.*;

import com.arjuna.mw.wscf.model.twophase.common.TwoPhaseResult;
import com.arjuna.mw.wscf.model.twophase.outcomes.*;

import com.arjuna.mw.wsas.activity.Outcome;

import com.arjuna.mw.wscf.common.Qualifier;

import com.arjuna.mw.wsas.exceptions.*;

import java.util.Hashtable;

import java.io.PrintWriter;
import java.io.IOException;

/**
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: ParticipantWrapper.java,v 1.1 2003/01/07 10:33:52 nmcl Exp $
 */

public class ParticipantWrapper implements Participant
{
    
    /**
     * Constructor.
     *
     * @param theResource is the proxy that allows us to call out to the
     * object.
     */

    public ParticipantWrapper (Participant theResource)
    {
	_resourceHandle = theResource;
	_responses = new Hashtable();
    }

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

    public Outcome processMessage (Message notification) throws WrongStateException, ProtocolViolationException, SystemException
    {
	Outcome out = (Outcome) _responses.get(notification);
	
	if (out == null)
	    out = _resourceHandle.processMessage(notification);
	
	return out;
    }
    
    /**
     * @exception SystemException Thrown if any error occurs.
     *
     * @return the unique identity for this participant.
     */

    public String identity () throws SystemException
    {
	return _resourceHandle.identity();
    }

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

    public boolean packState (OutputObjectState os)
    {
	return _resourceHandle.packState(os);
    }

    /**
     * Unpack the state of the participant from the object buffer.
     *
     * @return <code>true</code> if successful, <code>false</code> otherwise.
     */

    public boolean unpackState (InputObjectState os)
    {
	return _resourceHandle.unpackState(os);
    }

    public final void setResponse (Message notification, Outcome response)
    {
	_responses.put(notification, response);
    }

    private Participant _resourceHandle;
    private Hashtable   _responses;
    
}

