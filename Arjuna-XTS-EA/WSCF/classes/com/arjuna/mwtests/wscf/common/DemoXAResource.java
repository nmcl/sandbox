/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: DemoXAResource.java,v 1.1 2003/01/07 10:37:17 nmcl Exp $
 */

package com.arjuna.mwtests.wscf.common;

import javax.transaction.xa.*;

public class DemoXAResource implements XAResource
{
    
    public DemoXAResource ()
    {
    }
    
    public void commit (Xid xid, boolean onePhase) throws XAException
    {
	System.out.println("DemoXAResource.commit "+xid);
    }

    public void end (Xid xid, int flags) throws XAException
    {
	System.out.println("DemoXAResource.end "+xid);
    }

    public void forget (Xid xid) throws XAException
    {
	System.out.println("DemoXAResource.forget "+xid);
    }
    
    public int getTransactionTimeout () throws XAException
    {
	System.out.println("DemoXAResource.getTransactionTimeout");

	return 0;
    }
    
    public int prepare (Xid xid) throws XAException
    {
	System.out.println("DemoXAResource.prepare "+xid);

	return XAResource.XA_OK;
    }

    public Xid[] recover (int flag) throws XAException
    {
	System.out.println("DemoXAResource.recover "+flag);

	return null;
    }

    public void rollback (Xid xid) throws XAException
    {
	System.out.println("DemoXAResource.rollback "+xid);
    }

    public boolean setTransactionTimeout (int seconds) throws XAException
    {
	System.out.println("DemoXAResource.setTransactionTimeout "+seconds);

	return true;
    }

    public void start (Xid xid, int flags) throws XAException
    {
	System.out.println("DemoXAResource.start "+xid);
    }

    public boolean isSameRM (XAResource xares) throws XAException
    {
	System.out.println("DemoXAResource.isSameRM "+xares);
       
	return (xares == this);
    }
    
}
