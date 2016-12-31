/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: DummyXA.java,v 1.1 2000/02/25 14:07:58 nmcl Exp $
 */

import javax.transaction.xa.*;
import java.io.*;

/*
 * Currently XAResources must be serializable so we can
 * recreate them in the event of a failure. It is likely
 * that other mechanisms will be added later to remove
 * this necessity, although serialization will still be
 * supported.
 */

public class DummyXA implements XAResource, Serializable
{
    
public void DummyXA ()
    {
	_timeout = 0;  // no timeout
    }
    
public void commit (Xid xid, boolean onePhase) throws XAException
    {
	System.out.println("DummyXA.commit called");
    }

public void end (Xid xid, int flags) throws XAException
    {
	System.out.println("DummyXA.end called");
    }

public void forget (Xid xid) throws XAException
    {
	System.out.println("DummyXA.forget called");
    }
    
public int getTransactionTimeout () throws XAException
    {
	System.out.println("DummyXA.getTransactionTimeout called");

	return _timeout;
    }

public int prepare (Xid xid) throws XAException
    {
	System.out.println("DummyXA.prepare called");

	return XAResource.XA_OK;
    }

public Xid[] recover (int flag) throws XAException
    {
	System.out.println("DummyXA.recover called");

	return null;
    }

public void rollback (Xid xid) throws XAException
    {
	System.out.println("DummyXA.rollback called");
    }

public boolean setTransactionTimeout (int seconds) throws XAException
    {
	System.out.println("DummyXA.setTransactionTimeout called");

	_timeout = seconds;

	return true;
    }

public void start (Xid xid, int flags) throws XAException
    {
	System.out.println("DummyXA.start called");
    }

public boolean isSameRM (XAResource xares) throws XAException
   {
       System.out.println("DummyXA.isSameRM called");
       
       return (xares == this);
   }

private int _timeout;
    
};
