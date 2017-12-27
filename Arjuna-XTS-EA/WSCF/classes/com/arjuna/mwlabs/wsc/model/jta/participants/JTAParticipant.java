/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: JTAParticipant.java,v 1.2 2003/02/24 13:21:32 nmcl Exp $
 */

package com.arjuna.mwlabs.wsc.model.jta.participants;

import com.arjuna.wsc.messaging.PortReference;

import javax.transaction.xa.*;

public class JTAParticipant implements XAResource
{
    
    public JTAParticipant (PortReference address)
    {
	_address = address;
    }
    
    public void commit (Xid xid, boolean onePhase) throws XAException
    {
	System.out.println("JTAParticipant.commit "+xid);
    }

    public void end (Xid xid, int flags) throws XAException
    {
	System.out.println("JTAParticipant.end "+xid);
    }

    public void forget (Xid xid) throws XAException
    {
	System.out.println("JTAParticipant.forget "+xid);
    }
    
    public int getTransactionTimeout () throws XAException
    {
	System.out.println("JTAParticipant.getTransactionTimeout");

	return 0;
    }
    
    public int prepare (Xid xid) throws XAException
    {
	System.out.println("JTAParticipant.prepare "+xid);

	return XAResource.XA_OK;
    }

    public Xid[] recover (int flag) throws XAException
    {
	System.out.println("JTAParticipant.recover "+flag);

	return null;
    }

    public void rollback (Xid xid) throws XAException
    {
	System.out.println("JTAParticipant.rollback "+xid);
    }

    public boolean setTransactionTimeout (int seconds) throws XAException
    {
	System.out.println("JTAParticipant.setTransactionTimeout "+seconds);

	return true;
    }

    public void start (Xid xid, int flags) throws XAException
    {
	System.out.println("JTAParticipant.start "+xid);
    }

    public boolean isSameRM (XAResource xares) throws XAException
    {
	System.out.println("JTAParticipant.isSameRM "+xares);
       
	return (xares == this);
    }

    private PortReference _address;
    
}
