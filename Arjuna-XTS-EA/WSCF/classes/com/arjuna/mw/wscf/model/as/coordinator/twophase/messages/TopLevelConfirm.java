/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: TopLevelConfirm.java,v 1.1 2003/01/07 10:33:42 nmcl Exp $
 */

package com.arjuna.mw.wscf.model.as.coordinator.twophase.messages;

import com.arjuna.mw.wscf.common.CoordinatorId;
import com.arjuna.mw.wscf.common.Qualifier;

import com.arjuna.mw.wscf.model.as.coordinator.Message;

import com.arjuna.mw.wsas.exceptions.SystemException;

/**
 * The coordinator is top-level and is confirming. Any error at this point will
 * cause a heuristic.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: TopLevelConfirm.java,v 1.1 2003/01/07 10:33:42 nmcl Exp $
 */

public class TopLevelConfirm implements Message
{

    public TopLevelConfirm (CoordinatorId tid)
    {
	_tid = tid;
    }
    
    public TopLevelConfirm (CoordinatorId tid, Qualifier[] quals)
    {
	_tid = tid;
    }

    public boolean equals (Object o)
    {
	if (o == null)
	    return false;

	if (o instanceof TopLevelConfirm)
	    return true;
	else
	    return false;
    }

    /**
     * @return a printable version of the vote.
     */

    public String toString ()
    {
	return "org.w3c.wscf.twophase.messages.TopLevelConfirm";
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
