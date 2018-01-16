/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ParticipantAction.java,v 1.1 2003/02/03 11:01:29 nmcl Exp $
 */

package com.arjuna.mwlabs.wstx.model.as.twophase.resource;

import com.hp.mw.ts.arjuna.state.*;

import com.arjuna.mw.wstx.resource.Participant;

import com.arjuna.mw.wstx.common.*;

import com.arjuna.mw.wscf.common.Qualifier;

import com.arjuna.mw.wscf.model.as.coordinator.Message;
import com.arjuna.mw.wscf.model.as.coordinator.twophase.messages.*;
import com.arjuna.mw.wscf.model.as.coordinator.twophase.outcomes.*;

import com.arjuna.mw.wscf.model.twophase.common.*;
import com.arjuna.mw.wscf.model.twophase.outcomes.*;

import com.arjuna.mw.wsas.activity.Outcome;

import com.arjuna.mw.wsas.exceptions.SystemException;
import com.arjuna.mw.wsas.exceptions.WrongStateException;
import com.arjuna.mw.wsas.exceptions.ProtocolViolationException;

import com.arjuna.mw.wstx.exceptions.*;

import java.util.Hashtable;

/**
 * This class represents the two-phase aware participants that are enrolled
 * with the transaction. It wraps the actual participants that have two-phase
 * operations on them and translated to/from the low-level WSCF protocol.
 * 
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: ParticipantAction.java,v 1.1 2003/02/03 11:01:29 nmcl Exp $
 * @since 1.0.
 */

public class ParticipantAction implements com.arjuna.mw.wscf.model.as.coordinator.Participant
{

    public ParticipantAction (Participant res)
    {
	_resource = res;
    }
    
    public Outcome processMessage (Message notification) throws WrongStateException, ProtocolViolationException, SystemException
    {
	if (_resource == null)
	    return new CoordinationOutcome(TwoPhaseResult.FINISH_ERROR);

	Outcome result = new CoordinationOutcome(TwoPhaseResult.FINISH_ERROR);
	
	if ((notification instanceof NestedPrepare) || (notification instanceof TopLevelPrepare))
	{
	    result = doPrepare(notification.qualifiers());
	}

	if ((notification instanceof NestedConfirm) || (notification instanceof TopLevelConfirm))
	{
	    result = doCommit(notification.qualifiers());
	}
	
	if ((notification instanceof NestedCancel) || (notification instanceof TopLevelCancel))
	{
	    result = doAbort(notification.qualifiers());
	}

	if ((notification instanceof NestedOnePhaseCommit) || (notification instanceof TopLevelOnePhaseCommit))
	{
	    result = doOnePhaseCommit(notification.qualifiers());
	}

	if (notification instanceof ForgetHeuristic)
	{
	    result = doForget(notification.qualifiers());
	}
	
	return result;
    }

    /**
     * These methods are required so that the coordinator can serialise and
     * de-serialise information about the inferior during completion and
     * recovery.
     */

    public boolean packState (OutputObjectState os)
    {
	return true;
    }

    public boolean unpackState (InputObjectState os)
    {
	return true;
    }

    private final Outcome doPrepare (Qualifier[] qualifiers) throws WrongStateException, ProtocolViolationException, SystemException
    {
	Outcome result = new VoteCancel();
	
	try
	{
	    com.arjuna.mw.wstx.common.Vote v = _resource.prepare(qualifiers);
	    
	    if (v instanceof VoteCommit)
		return new VoteConfirm();
	    else
	    {
		if (v instanceof com.arjuna.mw.wstx.common.VoteReadOnly)
		    return new com.arjuna.mw.wscf.model.as.coordinator.twophase.outcomes.VoteReadOnly();
	    }
	}
	catch (InvalidParticipantException ex)
	{
	}
	catch (HeuristicHazardException ex)
	{
	    throw new ProtocolViolationException(ex.toString());
	}
	catch (HeuristicMixedException ex)
	{
	    throw new ProtocolViolationException(ex.toString());
	}

	return result;
    }

    public String identity () throws SystemException
    {
	return "ParticipantAction";
    }
    
    private final Outcome doCommit (Qualifier[] qualifiers) throws SystemException, WrongStateException
    {
	try
	{
	    _resource.commit(qualifiers);
	    
	    return new CoordinationOutcome(TwoPhaseResult.CONFIRMED);
	}
	catch (InvalidParticipantException ex)
	{
	    return new CoordinationOutcome(TwoPhaseResult.FINISH_ERROR);
	}
	catch (HeuristicHazardException ex)
	{
	    return new CoordinationOutcome(TwoPhaseResult.HEURISTIC_HAZARD);
	}
	catch (HeuristicMixedException ex)
	{
	    return new CoordinationOutcome(TwoPhaseResult.HEURISTIC_MIXED);
	}
	catch (HeuristicRollbackException ex)
	{
	    return new CoordinationOutcome(TwoPhaseResult.HEURISTIC_CANCEL);
	}
    }

    private final Outcome doOnePhaseCommit (Qualifier[] qualifiers) throws SystemException, WrongStateException
    {
	try
	{
	    _resource.commitOnePhase(qualifiers);
	    
	    return new CoordinationOutcome(TwoPhaseResult.CONFIRMED);
	}
	catch (InvalidParticipantException ex)
	{
	    return new CoordinationOutcome(TwoPhaseResult.FINISH_ERROR);
	}
	catch (HeuristicHazardException ex)
	{
	    return new CoordinationOutcome(TwoPhaseResult.HEURISTIC_HAZARD);
	}
	catch (HeuristicMixedException ex)
	{
	    return new CoordinationOutcome(TwoPhaseResult.HEURISTIC_MIXED);
	}
	catch (HeuristicRollbackException ex)
	{
	    return new CoordinationOutcome(TwoPhaseResult.HEURISTIC_CANCEL);
	}
    }

    private final Outcome doAbort (Qualifier[] qualifiers) throws WrongStateException, SystemException
    {
	try
	{
	    _resource.rollback(qualifiers);
	    
	    return new CoordinationOutcome(TwoPhaseResult.CANCELLED);
	}
	catch (InvalidParticipantException ex)
	{
	    return new CoordinationOutcome(TwoPhaseResult.FINISH_ERROR);
	}
	catch (HeuristicHazardException ex)
	{
	    return new CoordinationOutcome(TwoPhaseResult.HEURISTIC_HAZARD);
	}
	catch (HeuristicMixedException ex)
	{
	    return new CoordinationOutcome(TwoPhaseResult.HEURISTIC_MIXED);
	}
	catch (HeuristicCommitException ex)
	{
	    return new CoordinationOutcome(TwoPhaseResult.HEURISTIC_CANCEL);
	}
    }

    private final Outcome doForget (Qualifier[] qualifiers) throws WrongStateException, SystemException
    {
	try
	{
	    _resource.forget(qualifiers);
	    
	    return new CoordinationOutcome(TwoPhaseResult.FINISH_OK);
	}
	catch (InvalidParticipantException ex)
	{
	    return new CoordinationOutcome(TwoPhaseResult.FINISH_ERROR);
	}
    }

    private Participant _resource;
    
}
