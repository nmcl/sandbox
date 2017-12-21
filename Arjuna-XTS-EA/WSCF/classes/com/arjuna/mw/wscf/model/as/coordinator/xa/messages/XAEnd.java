/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: XAEnd.java,v 1.1 2003/01/07 10:33:43 nmcl Exp $
 */

package com.arjuna.mw.wscf.model.as.coordinator.xa.messages;

import com.arjuna.mw.wscf.common.CoordinatorId;
import com.arjuna.mw.wscf.common.Qualifier;

import com.arjuna.mw.wscf.model.as.coordinator.Message;

import com.arjuna.mw.wsas.exceptions.SystemException;

import com.arjuna.mwlabs.wscf.model.as.coordinator.jta.CoordinatorIdImple;

import javax.transaction.xa.Xid;

/**
 * The relationship between the coordinator and the participant is ending.
 * This may be temporary or permanent.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: XAEnd.java,v 1.1 2003/01/07 10:33:43 nmcl Exp $
 */

public class XAEnd implements Message
{

    public XAEnd (Xid tid, int flags)
    {
	this(tid, flags, null);
    }
    
    public XAEnd (Xid tid, int flags, Qualifier[] quals)
    {
	_tid = new CoordinatorIdImple(tid);
	_flags = flags;
    }

    public boolean equals (Object o)
    {
	if (o == null)
	    return false;

	if (o instanceof XAEnd)
	    return true;
	else
	    return false;
    }

    /**
     * @return a printable version of the vote.
     */

    public String toString ()
    {
	return "org.w3c.wscf.xa.messages.XAEnd";
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

    public int flags () throws SystemException
    {
	return _flags;
    }
    
    private CoordinatorId _tid;
    private int           _flags;

}
