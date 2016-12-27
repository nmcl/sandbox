/*
 * Copyright (C) 1999,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DummyXA.java,v 1.3 1999/05/14 10:18:18 nmcl Exp $
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
    
public void commit (Xid xid, boolean onePhase)
    {
	System.out.println("DummyXA.commit called");
    }

public void end (Xid xid, int flags)
    {
	System.out.println("DummyXA.end called");
    }

public void forget (Xid xid)
    {
	System.out.println("DummyXA.forget called");
    }
    
public int getTransactionTimeout ()
    {
	System.out.println("DummyXA.getTransactionTimeout called");

	return _timeout;
    }

public int prepare (Xid xid)
    {
	System.out.println("DummyXA.prepare called");

	return XAResource.XA_OK;
    }

public Xid[] recover (int flag)
    {
	System.out.println("DummyXA.recover called");

	return null;
    }

public void rollback (Xid xid)
    {
	System.out.println("DummyXA.rollback called");
    }

public void setTransactionTimeout (int seconds)
    {
	System.out.println("DummyXA.setTransactionTimeout called");

	_timeout = seconds;
    }

public void start (Xid xid, int flags)
    {
	System.out.println("DummyXA.start called");
    }

private int _timeout;
    
};
