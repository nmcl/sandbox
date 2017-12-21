/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: XARecover.java,v 1.1 2003/01/07 10:33:43 nmcl Exp $
 */

package com.arjuna.mw.wscf.model.as.coordinator.xa.messages;

import com.arjuna.mw.wscf.common.CoordinatorId;
import com.arjuna.mw.wscf.common.Qualifier;

import com.arjuna.mw.wscf.model.as.coordinator.Message;

import com.arjuna.mw.wsas.exceptions.SystemException;

/**
 * The transaction is recovering and a list of in doubt resources is
 * being requested.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: XARecover.java,v 1.1 2003/01/07 10:33:43 nmcl Exp $
 */

public class XARecover implements Message
{

    public XARecover (int flag)
    {
	this(flag, null);
    }
    
    public XARecover (int flag, Qualifier[] quals)
    {
	_flag = flag;
    }

    public boolean equals (Object o)
    {
	if (o == null)
	    return false;

	if (o instanceof XARecover)
	    return true;
	else
	    return false;
    }

    /**
     * @return a printable version of the vote.
     */

    public String toString ()
    {
	return "org.w3c.wscf.xa.messages.XARecover";
    }

    public String messageName () throws SystemException
    {
	return toString();
    }

    public Object coordinationSpecificData () throws SystemException
    {
	return new Integer(_flag);
    }

    public Qualifier[] qualifiers () throws SystemException
    {
	return null;
    }

    private int _flag;
    
}
