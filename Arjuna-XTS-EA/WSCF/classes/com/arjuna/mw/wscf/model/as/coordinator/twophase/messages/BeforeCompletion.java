/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: BeforeCompletion.java,v 1.1 2003/01/07 10:33:41 nmcl Exp $
 */

package com.arjuna.mw.wscf.model.as.coordinator.twophase.messages;

import com.arjuna.mw.wscf.common.CoordinatorId;
import com.arjuna.mw.wscf.common.Qualifier;

import com.arjuna.mw.wscf.model.as.coordinator.Message;

import com.arjuna.mw.wsas.exceptions.SystemException;

/**
 * A Synchronization participant is informed that the coordinator it is
 * registered with is about to complete the two-phase protocol and in what
 * state it finishing, i.e., confirmed or cancelled. The failure of the
 * participant at this stage will cause the coordinator to cancel if it is
 * not already doing so.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: BeforeCompletion.java,v 1.1 2003/01/07 10:33:41 nmcl Exp $
 */

public class BeforeCompletion implements Message
{

    public BeforeCompletion (CoordinatorId tid)
    {
	_tid = tid;
    }
    
    public BeforeCompletion (CoordinatorId tid, Qualifier[] quals)
    {
	_tid = tid;
    }

    public boolean equals (Object o)
    {
	if (o == null)
	    return false;

	if (o instanceof BeforeCompletion)
	    return true;
	else
	    return false;
    }

    /**
     * @return a printable version of the vote.
     */

    public String toString ()
    {
	return "org.w3c.wscf.twophase.messages.BeforeCompletion";
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
