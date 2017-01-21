/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DHThreadObject3a.java,v 1.2 1998/07/06 13:29:43 nmcl Exp $
 */

package JavaCosTransactionsTests;

import TestModule.*;
import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.JavaCosTransactions.*;
import java.util.Random;

import JavaCosTransactionsTests.TestException;
import java.lang.InterruptedException;

public class DHThreadObject3a extends Thread
{

public DHThreadObject3a (boolean doCommit)
    {
	if (doCommit)
	    _threadId = commitThreadId++;
	else
	    _threadId = abortThreadId++;
	
	_commit = doCommit;
    }

public void run ()
    {
	OTS_Current current = OTS.current();

	try
	{
	    current.begin();

	    Util.indent(_threadId, 0);
	    System.out.println("begin");

	    DistributedHammerWorker3.randomOperation(_threadId, 0);
	    DistributedHammerWorker3.randomOperation(_threadId, 0);

	    if (_commit)
		current.commit(false);
	    else
		current.rollback();

	    Util.indent(_threadId, 0);

	    if (_commit)
		System.out.println("end");
	    else
		System.out.println("abort");
	}
	catch (Exception e)
	{
	    System.err.println(e);
	}
    }

private int _threadId;
private boolean _commit;

private static int commitThreadId = 3;
private static int abortThreadId = 3;
    
};
