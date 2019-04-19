package com.hp.mwtests.ts.txoj.hammer;

/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Hammer.java,v 1.7 2003/06/19 11:56:27 nmcl Exp $
 */

import com.hp.mwtests.ts.txoj.*;
import com.arjuna.ats.arjuna.*;
import com.arjuna.ats.txoj.common.*;
import java.lang.Thread;
import java.util.Random;

import com.hp.mwtests.ts.txoj.common.exceptions.TestException;
import com.hp.mwtests.ts.txoj.common.resources.AtomicObject;
import com.hp.mwtests.ts.txoj.common.resources.HammerThreadedObject;
import com.arjuna.mwlabs.testframework.unittest.Test;
import com.arjuna.mwlabs.testframework.unittest.LocalHarness;

import java.lang.InterruptedException;

public class Hammer extends Test
{

public void run(String[] args)
    {
	HammerThreadedObject.object = new AtomicObject();
	HammerThreadedObject object1 = new HammerThreadedObject(2);
	HammerThreadedObject object2 = new HammerThreadedObject(-2);

	object1.start();
	object2.start();

	try
	{
	    object1.join();
	    object2.join();
	}
	catch (InterruptedException e)
	{
	}

	/* If we didn't deadlock, we'll get to here. */
	assertSuccess();
    }

public static void main(String[] args)
    {
        Hammer test = new Hammer();
        test.initialise(null, null, args, new LocalHarness());
        test.runTest();
    }
};
