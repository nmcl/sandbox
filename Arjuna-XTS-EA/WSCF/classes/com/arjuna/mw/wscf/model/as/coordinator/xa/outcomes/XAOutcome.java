/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: XAOutcome.java,v 1.1 2003/01/07 10:33:43 nmcl Exp $
 */

package com.arjuna.mw.wscf.model.as.coordinator.xa.outcomes;

import com.arjuna.mw.wsas.activity.Outcome;

import com.arjuna.mw.wsas.completionstatus.CompletionStatus;
import com.arjuna.mw.wsas.completionstatus.Failure;
import com.arjuna.mw.wsas.completionstatus.Success;

import com.arjuna.mw.wsas.exceptions.SystemException;

import org.w3c.dom.Document;

import javax.transaction.xa.XAException;

/**
 * The XAOutcome represents the final outcome of the coordination
 * event. The CompletionStatus and the actual two-phase status value are
 * returned.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: XAOutcome.java,v 1.1 2003/01/07 10:33:43 nmcl Exp $
 */

public class XAOutcome implements Outcome
{

    public XAOutcome ()
    {
	this(Success.instance(), null);
    }
    
    public XAOutcome (XAException ex)
    {
	this(Failure.instance(), ex);
    }

    public XAOutcome (CompletionStatus s)
    {
	this(s, null);
    }

    public XAOutcome (CompletionStatus s, XAException ex)
    {
	_status = s;
	_exp = ex;
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
	return "org.w3c.wscf.xa.outcomes.XAOutcome";
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
	return _exp;
    }

    public String toString ()
    {
	try
	{
	    return name()+":"+_exp;
	}
	catch (SystemException ex)
	{
	    return null;
	}
    }
    
    private CompletionStatus _status;
    private XAException      _exp;
    
}
