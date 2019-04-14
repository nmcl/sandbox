package com.hp.mwtests.ts.arjuna.licence;

/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: LicenceTest.java,v 1.6 2003/06/19 10:51:18 nmcl Exp $
 */

import com.arjuna.ats.arjuna.common.*;
import com.arjuna.mwlabs.testframework.unittest.Test;
import com.hp.mwtests.ts.arjuna.atomicaction.BadAction;

public class LicenceTest extends Test
{

public void run(String[] args)
    {
	String licence = null;
        String expectText = null;

	for (int i = 0; i < args.length; i++)
	{
	    if (args[i].equals("-licence"))
		licence = args[i+1];
            if (args[i].equals("-expect"))
                expectText = args[i+1];
	    if (args[i].equals("-help"))
	    {
		System.out.println("Usage: com.hp.mwtests.ts.arjuna.licence.LicenceTest -licence <licence> [-help]");
		assertFailure();
	    }
	}

	if (licence == null)
	    licence = arjPropertyManager.propertyManager.getProperty("com.arjuna.ats.arjuna.licence");

	if (licence == null)
	{
	    logInformation("Error - no licence specified.");
	    assertFailure();
	}

	Uid u = new Uid();  // make sure licence lets us get this far!
        String checkAll = com.arjuna.ats.internal.arjuna.common.LicenceType.checkAll();

        if ( ( expectText != null ) && ( !checkAll.equals( expectText ) ) )
        {
            assertFailure();
        }

	logInformation("Licence "+licence+" enables: "+checkAll);
        assertSuccess();
    }

public static void main(String[] args)
    {
        LicenceTest test = new LicenceTest();
        test.initialise(null, null, args, new com.arjuna.mwlabs.testframework.unittest.LocalHarness());
        test.run(args);
    }

};
