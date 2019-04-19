package com.hp.mwtests.ts.txoj.basic;

/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: RecoverableTest.java,v 1.6 2003/06/19 11:56:25 nmcl Exp $
 */

import com.hp.mwtests.ts.txoj.*;
import com.arjuna.ats.arjuna.*;
import com.arjuna.ats.txoj.common.*;

import com.hp.mwtests.ts.txoj.common.exceptions.TestException;
import com.hp.mwtests.ts.txoj.common.resources.RecoverableObject;
import com.arjuna.mwlabs.testframework.unittest.Test;
import com.arjuna.mwlabs.testframework.unittest.LocalHarness;

public class RecoverableTest extends Test
{
    
public void run(String[] args)
    {
	boolean passed = false;
	RecoverableObject foo = new RecoverableObject();
	int value = 0;

	AtomicAction A = new AtomicAction();

	A.begin();
	
	logInformation("value is "+foo.get());

	foo.set(2);

	logInformation("value is "+foo.get());

	A.abort();

	value = foo.get();

	logInformation("value is now "+value);

	if (value == 0)
	{
	    AtomicAction B = new AtomicAction();
	
	    B.begin();
	
	    logInformation("value is "+foo.get());

	    foo.set(4);

	    logInformation("value is "+foo.get());

	    B.commit();

	    value = foo.get();

	    logInformation("value is now "+value);

	    if (value == 4)
		passed = true;
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
        RecoverableTest test = new RecoverableTest();
        test.initialise(null, null, args, new LocalHarness());
        test.runTest();
    }
}

