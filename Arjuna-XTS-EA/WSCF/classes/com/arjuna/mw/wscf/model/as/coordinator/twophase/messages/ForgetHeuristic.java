/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ForgetHeuristic.java,v 1.1 2003/01/07 10:33:41 nmcl Exp $
 */

package com.arjuna.mw.wscf.model.as.coordinator.twophase.messages;

import com.arjuna.mw.wscf.common.CoordinatorId;
import com.arjuna.mw.wscf.common.Qualifier;

import com.arjuna.mw.wscf.model.as.coordinator.Message;

import com.arjuna.mw.wsas.exceptions.SystemException;

/**
 * The participant made a post-prepare choice that was contrary to that which
 * the coordinator told it to do. Hence it may have caused a non-atomic
 * (heuristic) outcome. If this happens, the participant *must* remember the
 * decision it took (persistently) until the coordinator tells it that it is
 * safe to forget.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: ForgetHeuristic.java,v 1.1 2003/01/07 10:33:41 nmcl Exp $
 */

public class ForgetHeuristic implements Message
{

    public ForgetHeuristic (CoordinatorId tid)
    {
	_tid = tid;
    }
    
    public ForgetHeuristic (CoordinatorId tid, Qualifier[] quals)
    {
	_tid = tid;
    }

    public boolean equals (Object o)
    {
	if (o == null)
	    return false;

	if (o instanceof ForgetHeuristic)
	    return true;
	else
	    return false;
    }

    /**
     * @return a printable version of the vote.
     */

    public String toString ()
    {
	return "org.w3c.wscf.twophase.messages.ForgetHeuristic";
    }

    public String messageName () throws SystemException
    {
	return toString();
    }

    public Object coordinationSpecificData () throws SystemException
    {
	return _tid;
    }

    public Qualifier[] qualifiers () throws SystemException
    {
	return null;
    }
    
    private CoordinatorId _tid;

}
