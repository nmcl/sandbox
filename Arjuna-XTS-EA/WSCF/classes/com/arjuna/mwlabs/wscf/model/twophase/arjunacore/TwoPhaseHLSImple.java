/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: TwoPhaseHLSImple.java,v 1.3 2003/03/04 12:55:57 nmcl Exp $
 */

package com.arjuna.mwlabs.wscf.model.twophase.arjunacore;

import com.arjuna.mwlabs.wscf.UserCoordinatorImple;

import com.arjuna.mw.wscf.model.twophase.outcomes.CoordinationOutcome;
import com.arjuna.mw.wscf.model.twophase.status.*;
import com.arjuna.mw.wscf.model.twophase.common.TwoPhaseResult;
import com.arjuna.mw.wscf.model.twophase.api.*;
import com.arjuna.mw.wscf.model.twophase.hls.TwoPhaseHLS;

import com.arjuna.mw.wscf.common.Environment;

import com.arjuna.mw.wsas.context.soap.SOAPContext;

import com.arjuna.mw.wsas.status.Status;
import com.arjuna.mw.wsas.status.Active;
import com.arjuna.mw.wsas.status.Unknown;
import com.arjuna.mw.wsas.status.NoActivity;
import com.arjuna.mw.wsas.status.Created;

import com.arjuna.mw.wscf.UserCoordinatorFactory;

import com.arjuna.mw.wscf.common.Qualifier;
import com.arjuna.mw.wscf.common.CoordinatorId;

import com.arjuna.mw.wscf.api.UserCoordinatorService;

import com.arjuna.mw.wsas.context.Context;

import com.hp.mw.ts.arjuna.common.Uid;

import com.hp.mw.ts.arjuna.coordinator.TwoPhaseCoordinator;
import com.hp.mw.ts.arjuna.coordinator.ActionStatus;

import com.arjuna.mw.wsas.UserActivityFactory;
import com.arjuna.mw.wsas.UserActivity;

import com.arjuna.mw.wsas.ActivityManagerFactory;

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
import com.arjuna.mw.wsas.exceptions.NoActivityException;
import com.arjuna.mw.wsas.exceptions.HLSError;

import com.arjuna.mw.wscf.exceptions.*;

import java.util.Hashtable;

/**
 * The ArjunaCore coordination service implementation.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: TwoPhaseHLSImple.java,v 1.3 2003/03/04 12:55:57 nmcl Exp $
 * @since 1.0.
 */

public class TwoPhaseHLSImple implements TwoPhaseHLS, UserCoordinatorService
{

    public TwoPhaseHLSImple ()
    {
	try
	{
	    ActivityManagerFactory.activityManager().addHLS((HLS) this);
	}
	catch (Exception ex)
	{
	    throw new HLSError(ex.toString());
	}

	_coordinatorService = new CoordinatorServiceImple();
	_coordManager = new CoordinatorControl();
    }
    
    public UserCoordinatorService coordinatorService ()
    {
	return this;
    }
    
    public UserCoordinator userCoordinator ()
    {
	return _coordinatorService;
    }
    
    public CoordinatorManager coordinatorManager ()
    {
	return _coordinatorService;
    }
    
    /**
     * An activity has begun and is active on the current thread.
     */

    public void begun () throws SystemException
    {
	_coordManager.begin();
    }

    /**
     * The current activity is completing with the specified completion status.
     *
     * @param CompletionStatus cs The completion status to use.
     *
     * @return The result of terminating the relationship of this HLS and
     * the current activity.
     */

    public Outcome complete (CompletionStatus cs) throws SystemException
    {
	return _coordManager.complete(cs);
    }	

    /**
     * The activity has been suspended.
     */

    public void suspended () throws SystemException
    {
	_coordManager.suspend();
    }	

    /**
     * The activity has been resumed on the current thread.
     */

    public void resumed () throws SystemException
    {
	_coordManager.resume();
    }	

    /**
     * The activity has completed and is no longer active on the current
     * thread.
     */

    public void completed () throws SystemException
    {
	_coordManager.completed();
    }

    /**
     * The HLS name.
     */

    public String identity () throws SystemException
    {
	return "TwoPhaseHLSImple";
    }

    /**
     * The activity service maintains a priority ordered list of HLS
     * implementations. If an HLS wishes to be ordered based on priority
     * then it can return a non-negative value: the higher the value,
     * the higher the priority and hence the earlier in the list of HLSes
     * it will appear (and be used in).
     *
     * @return a positive value for the priority for this HLS, or zero/negative
     * if the order is not important.
     */

    public int priority () throws SystemException
    {
	return 0;
    }

    /**
     * Return the context augmentation for this HLS, if any on the current
     * activity.
     *
     * @param ActivityHierarchy current The handle on the current activity
     * hierarchy. The HLS may use this when determining what information to
     * place in its context data.
     *
     * @return a context object or null if no augmentation is necessary.
     */

    /*
     * TODO This needs refactoring. It's true that the context format should
     * be configurable to allow the same implementation to be used in
     * different protocols (e.g., 2PC mapping to OTS or original Arjuna could
     * use the same protocol implementation, but the context formats are
     * different.) However, is this the best way of doing that?
     */

    public Context context () throws SystemException
    {
	String contextImple = System.getProperty(Environment.TWO_PHASE_CONTEXT);

	if (contextImple != null)
	{
	    try
	    {
		Class c = Class.forName(contextImple);
		SOAPContext ctx = (SOAPContext) c.newInstance();
		
		ctx.initialise(_coordManager.currentCoordinator());

		return ctx;
	    }
	    catch (Exception ex)
	    {
		throw new SystemException(ex.toString());
	    }
	}
	else
	    return new com.arjuna.mwlabs.wscf.model.twophase.arjunacore.context.soap.ArjunaContextImple(_coordManager.currentCoordinator());
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
	return _coordManager.coordinate(cs);
    }

    /**
     * @exception SystemException Thrown if any error occurs.
     *
     * @return the status of the current coordinator. If there is no
     * activity associated with the thread then NoActivity
     * will be returned.
     *
     * @see com.arjuna.mw.wsas.status.Status
     */

    public com.arjuna.mw.wsas.status.Status status () throws SystemException
    {
	return _coordManager.status();
    }

    /**
     * Not supported by basic ArjunaCore.
     *
     * @exception SystemException Thrown if any error occurs.
     *
     * @return the complete list of qualifiers that have been registered with
     * the current coordinator.
     */
    
    public Qualifier[] qualifiers () throws NoCoordinatorException, SystemException
    {
	return _coordManager.qualifiers();
    }

    /**
     * @exception SystemException Thrown if any error occurs.
     *
     * @return The unique identity of the current coordinator.
     */

    public CoordinatorId identifier () throws NoCoordinatorException, SystemException
    {
	return _coordManager.identifier();
    }

    public static String className ()
    {
	return TwoPhaseHLSImple.class.getName();
    }

    private CoordinatorControl      _coordManager;
    private CoordinatorServiceImple _coordinatorService;

}
