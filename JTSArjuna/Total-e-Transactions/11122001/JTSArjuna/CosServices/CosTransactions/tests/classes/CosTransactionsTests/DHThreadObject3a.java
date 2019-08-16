/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: DHThreadObject3a.java,v 1.1 2000/02/25 14:08:47 nmcl Exp $
 */

package CosTransactionsTests;

import TestModule.*;
import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.CosTransactions.*;
import java.util.Random;

import CosTransactionsTests.TestException;
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
