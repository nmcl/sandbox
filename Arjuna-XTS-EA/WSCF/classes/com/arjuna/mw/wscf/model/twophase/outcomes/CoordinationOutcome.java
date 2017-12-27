/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: CoordinationOutcome.java,v 1.1 2003/01/07 10:33:45 nmcl Exp $
 */

package com.arjuna.mw.wscf.model.twophase.outcomes;

import com.arjuna.mw.wscf.model.twophase.common.TwoPhaseResult;

import com.arjuna.mw.wsas.activity.Outcome;

import com.arjuna.mw.wsas.completionstatus.CompletionStatus;
import com.arjuna.mw.wsas.completionstatus.Success;

import com.arjuna.mw.wsas.exceptions.SystemException;

import org.w3c.dom.Document;

/**
 * The CoordinationOutcome represents the final outcome of the coordination
 * event. The CompletionStatus and the actual two-phase status value are
 * returned.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: CoordinationOutcome.java,v 1.1 2003/01/07 10:33:45 nmcl Exp $
 */

public class CoordinationOutcome implements Outcome
{
    
    public CoordinationOutcome (int twophase)
    {
	this(Success.instance(), twophase);
    }

    public CoordinationOutcome (CompletionStatus s, int twophase)
    {
	_status = s;
	_twophaseOutcome = twophase;
    }

    /**
     * @return the two-phase status result.
     * @see com.arjuna.mw.wscf.model.twophase.common.TwoPhaseResult
     */

    public final int result ()
    {
	return _twophaseOutcome;
    }
    
    /**
     * Give a name for this outcome.
     *
     * @exception SystemException Thrown if an error occurs.
     * @return some implementation specific name for the Outcome. Typically
     * this will be the only thing necessary to determine the transaction's
     * outcome, e.g., "RolledBack".
     */

    public String name () throws SystemException
    {
	return "org.w3c.wscf.twophase.outcomes.CoordinationOutcome";
    }

    /**
     * The state in which the activity completed.
     *
     * @exception SystemException Thrown if an error occurs.
     *
     * @return the final completion status of the transaction. Any additional
     * information (e.g., exception types) may be provided by the data method.
     */
 
    public CompletionStatus completedStatus () throws SystemException
    {
	return _status;
    }
 
    /**
     * Outcome specific information that can be used to determine finer
     * level details about the outcome instance.
     *
     * @exception SystemException Thrown if an error occurs.
     *
     * @return completion specific data for this activity. Examples include
     * the type of failure exception that was thrown by the implementation
     * (e.g., HeuristicMixed).
     */

    public Object data () throws SystemException
    {
	return null;
    }

    public String toString ()
    {
	return TwoPhaseResult.stringForm(_twophaseOutcome);
    }
    
    private CompletionStatus _status;
    private int              _twophaseOutcome;
    
}