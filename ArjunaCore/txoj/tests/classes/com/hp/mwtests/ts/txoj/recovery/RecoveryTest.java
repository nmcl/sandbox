/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: RecoveryTest.java,v 1.5 2003/06/19 11:56:28 nmcl Exp $
 */

package com.hp.mwtests.ts.txoj.recovery;

import com.hp.mwtests.ts.txoj.*;
import com.arjuna.ats.arjuna.*;
import com.arjuna.ats.arjuna.coordinator.*;
import com.arjuna.ats.arjuna.state.*;
import com.arjuna.ats.arjuna.gandiva.*;
import com.arjuna.ats.arjuna.common.*;
import java.util.Random;
import java.lang.Math;

import com.hp.mwtests.ts.txoj.TestException;
import com.arjuna.mwlabs.testframework.unittest.Test;

public class RecoveryTest extends Test
{

    public void run (String[] args)
    {
	try
	{
	    AtomicObject obj = new AtomicObject();
	    Uid objRef = obj.get_uid();
	
	    AtomicAction A = new AtomicAction();
	
	    A.begin();
	
	    obj.set(1234);
	
	    A.commit();
	
	    AtomicObject recObj = new AtomicObject(objRef);
	
	    AtomicAction B = new AtomicAction();

	    B.begin();
	
	    if (recObj.get() == 1234)
	    {
		System.out.print("Passed.");

		assertSuccess();
	    }
	    else
	    {
		assertFailure();
	    }

	    B.abort();
	}
	catch (Exception ex)
	{
	    assertFailure();
	}
    }

    public static void main (String args[])
    {
	RecoveryTest rt = new RecoveryTest();

	rt.initialise(null, null, args, new com.arjuna.mwlabs.testframework.unittest.LocalHarness());
	
	rt.run(args);
    }
    
}
