/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: XAForget.java,v 1.1 2003/01/07 10:33:43 nmcl Exp $
 */

package com.arjuna.mw.wscf.model.as.coordinator.xa.messages;

import com.arjuna.mw.wscf.common.CoordinatorId;
import com.arjuna.mw.wscf.common.Qualifier;

import com.arjuna.mw.wscf.model.as.coordinator.Message;

import com.arjuna.mw.wsas.exceptions.SystemException;

import com.arjuna.mwlabs.wscf.model.as.coordinator.jta.CoordinatorIdImple;

import javax.transaction.xa.Xid;

/**
 * The participant caused a heuristic and as a result has had to maintain
 * information about it until now. This message from the coordinator
 * informs the participant that it can forget about the heuristic.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: XAForget.java,v 1.1 2003/01/07 10:33:43 nmcl Exp $
 */

public class XAForget implements Message
{

    public XAForget (Xid tid)
    {
	this(tid, null);
    }
    
    public XAForget (Xid tid, Qualifier[] quals)
    {
	_tid = new CoordinatorIdImple(tid);
    }

    public boolean equals (Object o)
    {
	if (o == null)
	    return false;

	if (o instanceof XAForget)
	    return true;
	else
	    return false;
    }

    /**
     * @return a printable version of the vote.
     */

    public String toString ()
    {
	return "org.w3c.wscf.xa.messages.XAForget";
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
