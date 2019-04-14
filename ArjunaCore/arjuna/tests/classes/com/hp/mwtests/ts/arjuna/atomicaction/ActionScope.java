package com.hp.mwtests.ts.arjuna.atomicaction;

/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ActionScope.java,v 1.6 2003/06/19 10:51:18 nmcl Exp $
 */

import com.arjuna.ats.arjuna.AtomicAction;
import com.arjuna.ats.arjuna.coordinator.*;
import com.arjuna.ats.arjuna.common.*;
import com.arjuna.mwlabs.testframework.unittest.Test;

public class ActionScope extends Test
{

public void run(String[] args)
    {
        try
        {
            test();

            assertSuccess();
        }
        catch (Exception e)
        {
            logInformation("Unexpected exception thrown - "+e);
            e.printStackTrace(System.err);
            assertFailure();
        }


	System.gc();

	System.runFinalization();
    }

private void test () throws Exception
    {
	AtomicAction A = new AtomicAction();

	A.begin();

	A = null;

	System.gc();
    }

public static void main(String[] args)
    {
        ActionScope test = new ActionScope();
    	test.initialise(null, null, args, new com.arjuna.mwlabs.testframework.unittest.LocalHarness());
    	test.run(args);
    }
};
