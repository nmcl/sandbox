package com.hp.mwtests.ts.txoj.basic;

/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: AtomicTest.java,v 1.6 2003/06/19 11:56:25 nmcl Exp $
 */

import com.hp.mwtests.ts.txoj.*;
import com.arjuna.ats.arjuna.*;
import com.arjuna.ats.txoj.common.*;
import com.arjuna.ats.arjuna.common.*;

import com.hp.mwtests.ts.txoj.common.exceptions.TestException;
import com.hp.mwtests.ts.txoj.common.resources.AtomicObject;
import com.arjuna.mwlabs.testframework.unittest.Test;
import com.arjuna.mwlabs.testframework.unittest.LocalHarness;

public class AtomicTest extends Test
{
    
public void run(String[] args)
    {
	AtomicObject foo = new AtomicObject();
	Uid u = foo.get_uid();
	int v;
	
	AtomicAction A = new AtomicAction();

	try
	{
	    A.begin();
	    
	    logInformation("current object value is "+foo.get());
	    
	    foo.set(2);

	    logInformation("object value is now "+foo.get());

	    A.commit();

	    int finalVal = foo.get();
	    
	    logInformation("final object value is now "+finalVal);

	    if (finalVal == 2)
		logInformation("This is correct.");
	    else
	    {
		logInformation("This is incorrect.");
		assertFailure();
	    }

	    logInformation("\nStarting new action.");
	
	    foo = new AtomicObject(u);
	
	    A = new AtomicAction();
	
	    A.begin();

	    logInformation("current object value is "+foo.get());
	
	    foo.set(4);

	    A.commit();

	    finalVal = foo.get();
	    
	    logInformation("final object value is "+finalVal);

	    if (finalVal == 4)
		logInformation("This is correct.");
	    else
            {
		logInformation("This is incorrect.");
                assertFailure();
            }
	}
	catch (TestException e)
	{
	    logInformation("AtomicObject exception raised.");
            e.printStackTrace(System.err);
            assertFailure();

	    A.abort();
	}

        assertSuccess();
    }

public static void main(String[] args)
    {
        AtomicTest test = new AtomicTest();
        test.initialise( null, null, args, new LocalHarness() );
        test.runTest();
    }
};
