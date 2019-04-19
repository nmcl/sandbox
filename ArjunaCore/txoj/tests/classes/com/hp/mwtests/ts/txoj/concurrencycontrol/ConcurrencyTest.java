package com.hp.mwtests.ts.txoj.concurrencycontrol;

/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ConcurrencyTest.java,v 1.6 2003/06/19 11:56:26 nmcl Exp $
 */

import com.hp.mwtests.ts.txoj.*;
import com.arjuna.ats.arjuna.*;
import com.arjuna.ats.arjuna.coordinator.*;
import com.arjuna.ats.txoj.common.*;
import com.arjuna.ats.arjuna.common.*;

import com.hp.mwtests.ts.txoj.common.exceptions.TestException;
import com.hp.mwtests.ts.txoj.common.resources.AtomicObject;
import com.arjuna.mwlabs.testframework.unittest.Test;
import com.arjuna.mwlabs.testframework.unittest.LocalHarness;

public class ConcurrencyTest extends Test
{
    
public void run(String[] args)
    {
	AtomicObject foo = null;
	Uid u = null;
	
	for (int i = 0; i < args.length; i++)
	{
	    if (args[i].compareTo("-uid") == 0)
	    {
		u = new Uid(args[i+1]);

		if (!u.valid())
		{
		    logInformation("Invalid uid.");
		    assertFailure();
		}
	    }
	    if (args[i].compareTo("-help") == 0)
	    {
		System.out.println("Usage: [-uid <uid>] [-help]");
		assertFailure();
	    }
	}

	if (u == null)
	    foo = new AtomicObject();
	else
	    foo = new AtomicObject(u);

	logInformation("Starting top-level action.");

	AtomicAction A = new AtomicAction();

	try
	{
	    A.begin();

	    logInformation("Current atomic object state: " + foo.get());
	
	    foo.set(7);

	    if (u == null)
	    {
		logInformation("Now waiting for 20 seconds.");

		try
		{
		    Thread.sleep(20000);
		}
		catch (InterruptedException e)
		{
		}
	    }

	    logInformation("\nCommitting top-level action.");

	    if (A.commit() != ActionStatus.COMMITTED)
            {
		logInformation("Error when committing action.");
                assertFailure();
            }
	    else
            {
		logInformation("Action committed.");
            }
	}
	catch (TestException e)
	{
	    logInformation("Could not set state. Aborting action.");
            assertFailure();
	    A.abort();
	}

        assertSuccess();
    }

public static void main(String[] args)
    {
        ConcurrencyTest test = new ConcurrencyTest();
        test.initialise( null, null, args, new LocalHarness() );
        test.runTest();
    }
};
