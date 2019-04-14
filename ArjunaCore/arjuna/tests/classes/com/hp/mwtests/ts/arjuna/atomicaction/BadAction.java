package com.hp.mwtests.ts.arjuna.atomicaction;

/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: BadAction.java,v 1.6 2003/06/19 10:51:18 nmcl Exp $
 */

import com.arjuna.ats.arjuna.AtomicAction;
import com.arjuna.ats.arjuna.coordinator.*;
import com.arjuna.ats.arjuna.common.*;
import com.arjuna.mwlabs.testframework.unittest.Test;

public class BadAction extends Test
{

public void run(String[] args)
    {
        try
        {
            AtomicAction A = new AtomicAction();
            AtomicAction B = new AtomicAction();

            logInformation("Starting top-level action.");

            A.begin();

            logInformation(A.toString());

            logInformation("Starting nested action.");

            B.begin();

            logInformation(B.toString());

            logInformation("Committing top-level action. This should fail.");

            A.commit();

            logInformation("Committing nested action. This should fail.");

            B.commit();

            BasicAction current = BasicAction.Current();

            logInformation("Current action is " + current);

            assertSuccess();
        }
        catch (Exception e)
        {
            logInformation("Unexpected Exception - "+e);
            e.printStackTrace(System.err);
            assertFailure();
        }
    }

public static void main(String[] args)
    {
        BadAction test = new BadAction();
        test.initialise(null, null, args, new com.arjuna.mwlabs.testframework.unittest.LocalHarness());
        test.run(args);
    }

};
