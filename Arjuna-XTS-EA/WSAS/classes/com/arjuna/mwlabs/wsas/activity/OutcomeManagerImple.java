/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: OutcomeManagerImple.java,v 1.1 2002/11/25 10:51:45 nmcl Exp $
 */

package com.arjuna.mwlabs.wsas.activity;

import com.arjuna.mw.wsas.UserActivityFactory;
import com.arjuna.mw.wsas.UserActivity;

import com.arjuna.mw.wsas.activity.Outcome;
import com.arjuna.mw.wsas.activity.OutcomeManager;

import com.arjuna.mw.wsas.completionstatus.CompletionStatus;

import com.arjuna.mw.wsas.exceptions.SystemException;
import com.arjuna.mw.wsas.exceptions.NoActivityException;
import com.arjuna.mw.wsas.exceptions.HLSException;

import org.w3c.dom.Document;

/**
 * The Outcome represents the final outcome of the activity. Because
 * different HLS implementations will have different requirements
 * on what they can or cannot return, this interface is deliberately
 * generic.
 */

public class OutcomeManagerImple implements OutcomeManager
{

    /**
     * Even if exceptions occur just remember the equivalent exception
     * and keep telling all other HLSes that the activity is completing.
     */

    public Outcome processOutcome (Outcome current, Outcome next) throws SystemException
    {
	if (current == null)
	    return next;
	
	if (next == null)
	    return current;

	if (current instanceof CompositeOutcomeImple)
	{
	    ((CompositeOutcomeImple) current).add(next);

	    return current;
	}
	else
	{
	    try
	    {
		CompositeOutcomeImple toReturn = new CompositeOutcomeImple(UserActivityFactory.userActivity().getCompletionStatus());
	    
		toReturn.add(current);
		toReturn.add(next);

		return toReturn;
	    }
	    catch (NoActivityException ex)
	    {
		throw new SystemException(ex.toString());
	    }
	}
    }
    
}
