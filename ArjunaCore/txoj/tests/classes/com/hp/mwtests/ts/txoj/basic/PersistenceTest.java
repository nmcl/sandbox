package com.hp.mwtests.ts.txoj.basic;

/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: PersistenceTest.java,v 1.6 2003/06/19 11:56:25 nmcl Exp $
 */

import com.hp.mwtests.ts.txoj.*;
import com.arjuna.ats.arjuna.ArjunaNames;
import com.arjuna.ats.arjuna.objectstore.ObjectStore;
import com.arjuna.ats.arjuna.*;
import com.arjuna.ats.arjuna.state.*;
import com.arjuna.ats.arjuna.common.*;
import com.arjuna.ats.txoj.common.*;
import java.io.*;

import com.hp.mwtests.ts.txoj.common.exceptions.TestException;
import com.arjuna.ats.arjuna.exceptions.ObjectStoreException;
import com.arjuna.mwlabs.testframework.unittest.Test;
import com.arjuna.mwlabs.testframework.unittest.LocalHarness;

public class PersistenceTest extends Test
{
    
public void run(String[] args)
    {
	boolean passed = false;

	try
	{
	    ObjectStore store = new ObjectStore(ArjunaNames.Implementation_ObjectStore_ShadowingStore());
	    OutputObjectState state = new OutputObjectState();
	    Uid u = new Uid();

	    logInformation("Uid is "+u);

	    if (store.write_committed(u, "/StateManager/LockManager/foo", state))
	    {
		logInformation("written ok");

		passed = true;
	    }
	    else
		logInformation("write error");

	    if (passed)
	    {
		passed = false;

		/*
		 * Now try to read.
		 */

		InputObjectState inputState = store.read_committed(u, "/StateManager/LockManager/foo");

		if (inputState != null)
		{
		    logInformation("read ok");

		    passed = true;
		}
		else
		    logInformation("read error");
	    }
	}
	catch (ObjectStoreException e)
	{
	    logInformation(e.getMessage());

	    passed = false;
	}

	if (passed)
        {
	    logInformation("Test passed");
            assertSuccess();
        }
	else
        {
	    logInformation("Test failed");
            assertFailure();
        }
    }

public static void main(String[] args)
    {
        PersistenceTest test = new PersistenceTest();
        test.initialise(null, null, args, new LocalHarness());
        test.runTest();
    }
}
