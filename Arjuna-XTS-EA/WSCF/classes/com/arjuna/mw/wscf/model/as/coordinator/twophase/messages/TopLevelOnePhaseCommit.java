/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: TopLevelOnePhaseCommit.java,v 1.1 2003/01/07 10:33:42 nmcl Exp $
 */

package com.arjuna.mw.wscf.model.as.coordinator.twophase.messages;

import com.arjuna.mw.wscf.common.CoordinatorId;
import com.arjuna.mw.wscf.common.Qualifier;

import com.arjuna.mw.wscf.model.as.coordinator.Message;

import com.arjuna.mw.wsas.exceptions.SystemException;

/**
 * If only a single participant is registered with a two-phase coordinator
 * then it is possible for the coordinator to optimise the commit stage and
 * not have to execute two phases: the outcome of the coordination is
 * ultimately in the hands of the participant and not the coordinator. So,
 * the coordinator can send the participant a message telling it to commit
 * without having gone through a previous prepare stage.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: TopLevelOnePhaseCommit.java,v 1.1 2003/01/07 10:33:42 nmcl Exp $
 */

public class TopLevelOnePhaseCommit implements Message
{

    public TopLevelOnePhaseCommit (CoordinatorId tid)
    {
	_tid = tid;
    }
    
    public TopLevelOnePhaseCommit (CoordinatorId tid, Qualifier[] quals)
    {
	_tid = tid;
    }

    public boolean equals (Object o)
    {
	if (o == null)
	    return false;

	if (o instanceof TopLevelOnePhaseCommit)
	    return true;
	else
	    return false;
    }

    /**
     * @return a printable version of the vote.
     */

    public String toString ()
    {
	return "org.w3c.wscf.twophase.messages.TopLevelOnePhaseCommit";
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
