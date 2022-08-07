/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: DummyXA.java,v 1.1.2.1 2000/12/13 14:41:39 nmcl Exp $
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
    }

public void end (Xid xid, int flags) throws XAException
    {
    }

public void forget (Xid xid) throws XAException
    {
    }
    
public int getTransactionTimeout () throws XAException
    {

	return _timeout;
    }

public int prepare (Xid xid) throws XAException
    {
	return XAResource.XA_OK;
    }

public Xid[] recover (int flag) throws XAException
    {
	return null;
    }

public void rollback (Xid xid) throws XAException
    {
    }

public boolean setTransactionTimeout (int seconds) throws XAException
    {
	_timeout = seconds;

	return true;
    }

public void start (Xid xid, int flags) throws XAException
    {
    }

public boolean isSameRM (XAResource xares) throws XAException
   {
       return (xares == this);
   }

private int _timeout;
    
};
