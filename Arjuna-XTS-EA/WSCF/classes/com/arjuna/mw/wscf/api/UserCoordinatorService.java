/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: UserCoordinatorService.java,v 1.1 2003/01/07 10:33:33 nmcl Exp $
 */

package com.arjuna.mw.wscf.api;

import com.arjuna.mw.wsas.context.Context;

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

/**
 * This interface defines the operations that implementations of coordination
 * protocols must provide. The basic interface does not imply any specific
 * coordination protocol.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: UserCoordinatorService.java,v 1.1 2003/01/07 10:33:33 nmcl Exp $
 * @since 1.0.
 */

public interface UserCoordinatorService
{

    /**
     * @exception SystemException Thrown if any error occurs.
     *
     * @return the status of the current coordinator. If there is no
     * activity associated with the thread then NoActivity
     * will be returned.
     *
     * @see com.arjuna.mw.wsas.status.Status
     */

    public com.arjuna.mw.wsas.status.Status status () throws SystemException;

    /**
     * If the application requires and if the coordination protocol supports
     * it, then this method can be used to execute a coordination protocol on
     * the currently enlisted participants at any time prior to the termination
     * of the coordination scope.
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

    public Outcome coordinate (CompletionStatus cs) throws WrongStateException, ProtocolViolationException, NoCoordinatorException, SystemException;

    /**
     * @exception SystemException Thrown if any error occurs.
     *
     * @return the complete list of qualifiers that have been registered with
     * the current coordinator.
     */

    public Qualifier[] qualifiers () throws NoCoordinatorException, SystemException;

    /**
     * @exception SystemException Thrown if any error occurs.
     *
     * @return The unique identity of the current coordinator.
     */

    public CoordinatorId identifier () throws NoCoordinatorException, SystemException;
    
}
