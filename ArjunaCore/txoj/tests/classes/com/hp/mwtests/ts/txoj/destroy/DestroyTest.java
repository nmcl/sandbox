package com.hp.mwtests.ts.txoj.destroy;

/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: DestroyTest.java,v 1.6 2003/06/19 11:56:27 nmcl Exp $
 */

import com.hp.mwtests.ts.txoj.*;
import com.arjuna.ats.arjuna.*;
import com.arjuna.ats.txoj.common.*;
import com.arjuna.ats.arjuna.coordinator.*;
import com.arjuna.ats.arjuna.common.*;

import com.hp.mwtests.ts.txoj.common.exceptions.TestException;
import com.hp.mwtests.ts.txoj.common.resources.AtomicObject;
import com.arjuna.mwlabs.testframework.unittest.Test;
import com.arjuna.mwlabs.testframework.unittest.LocalHarness;

public class DestroyTest extends Test
{

public void run(String[] args)
    {
	AtomicObject atomicObject = new AtomicObject();
	Uid u = atomicObject.get_uid();
	AtomicAction a = new AtomicAction();
	
	a.begin();
	
	try
	{
	    atomicObject.set(10);
	}
	catch (TestException e)
	{
	    logInformation("set : failed");
            logInformation("Unexpected exception - "+e);
            e.printStackTrace(System.err);
	    assertFailure();
	}
	
	logInformation("set : ok");

	if (!atomicObject.destroy())
	{
	    logInformation("destroy : failed");

	    a.abort();
	    
	    assertFailure();
	}

	a.commit();
	
	atomicObject = new AtomicObject(u);
	
	boolean passed = false;

	try
	{
	    int val = atomicObject.get();
	    
	    if (val != -1)
	    {
		logInformation("got : "+val);

		logInformation("destroy did not work!");
	    }
	    else
	    {
		System.out.println("object destroyed!");

		passed = true;
	    }
	}
	catch (TestException e)
	{
	    System.out.println("object destroyed!");

	    passed = true;
	}

	if (passed)
	    System.out.println("Test passed");
	else
        {
	    System.out.println("Test failed");

            assertFailure();
        }

        assertSuccess();
    }

public static void main(String[] args)
    {
        DestroyTest test = new DestroyTest();
        test.initialise(null, null, args, new LocalHarness());
        test.runTest();
    }
}
