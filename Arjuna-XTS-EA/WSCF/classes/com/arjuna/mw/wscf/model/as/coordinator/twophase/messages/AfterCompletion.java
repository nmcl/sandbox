/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: AfterCompletion.java,v 1.1 2003/01/07 10:33:41 nmcl Exp $
 */

package com.arjuna.mw.wscf.model.as.coordinator.twophase.messages;

import com.arjuna.mw.wscf.common.CoordinatorId;
import com.arjuna.mw.wscf.common.Qualifier;

import com.arjuna.mw.wscf.model.as.coordinator.Message;

import com.arjuna.mw.wsas.completionstatus.CompletionStatus;

import com.arjuna.mw.wsas.exceptions.SystemException;

/**
 * A Synchronization participant is informed that the coordinator it is
 * registered with has completed the two-phase protocol and in what state
 * it finished, i.e., confirmed or cancelled. Any failures by the participant
 * at this stage have no affect on the coordinator.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: AfterCompletion.java,v 1.1 2003/01/07 10:33:41 nmcl Exp $
 * @since WSCF 1.0
 */

public class AfterCompletion implements Message
{

    public AfterCompletion (CoordinatorId tid, CompletionStatus cs)
    {
	_tid = tid;
	_cs = cs;
    }
    
    public AfterCompletion (CoordinatorId tid, CompletionStatus cs, Qualifier[] quals)
    {
	_tid = tid;
	_cs = cs;
    }

    public boolean equals (Object o)
    {
	if (o == null)
	    return false;

	if (o instanceof AfterCompletion)
	    return true;
	else
	    return false;
    }

    /**
     * @return a printable version of the vote.
     */

    public String toString ()
    {
	return "org.w3c.wscf.twophase.messages.AfterCompletion";
    }

    public String messageName () throws SystemException
    {
	return toString();
    }

    public Object coordinationSpecificData () throws SystemException
    {
	return _tid;
    }

    public CompletionStatus status () throws SystemException
    {
	return _cs;
    }
    
    public Qualifier[] qualifiers () throws SystemException
    {
	return null;
    }
    
    private CoordinatorId    _tid;
    private CompletionStatus _cs;
    
}
