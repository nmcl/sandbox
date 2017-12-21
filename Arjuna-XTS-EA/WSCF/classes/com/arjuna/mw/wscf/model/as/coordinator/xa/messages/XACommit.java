/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: XACommit.java,v 1.1 2003/01/07 10:33:43 nmcl Exp $
 */

package com.arjuna.mw.wscf.model.as.coordinator.xa.messages;

import com.arjuna.mw.wscf.common.CoordinatorId;
import com.arjuna.mw.wscf.common.Qualifier;

import com.arjuna.mw.wscf.model.as.coordinator.Message;

import com.arjuna.mw.wsas.exceptions.SystemException;

import com.arjuna.mwlabs.wscf.model.as.coordinator.jta.CoordinatorIdImple;

import javax.transaction.xa.Xid;

/**
 * The coordinator is committing. Any error at this point will
 * cause a heuristic.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: XACommit.java,v 1.1 2003/01/07 10:33:43 nmcl Exp $
 */

public class XACommit implements Message
{

    public XACommit (Xid tid, boolean onePhase)
    {
	this(tid, onePhase, null);
    }
    
    public XACommit (Xid tid, boolean onePhase, Qualifier[] quals)
    {
	_tid = new CoordinatorIdImple(tid);
	_onePhase = onePhase;
    }

    public boolean equals (Object o)
    {
	if (o == null)
	    return false;

	if (o instanceof XACommit)
	    return true;
	else
	    return false;
    }

    /**
     * @return a printable version of the vote.
     */

    public String toString ()
    {
	return "org.w3c.wscf.xa.messages.XACommit";
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

    public boolean onePhaseCommit () throws SystemException
    {
	return _onePhase;
    }
    
    private CoordinatorId _tid;
    private boolean       _onePhase;

}
