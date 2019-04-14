/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Hewlett-Packard Arjuna Labs,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Performance1.java,v 1.6 2003/06/19 10:51:19 nmcl Exp $
 */

package com.hp.mwtests.ts.arjuna.performance;

import com.arjuna.ats.arjuna.AtomicAction;
import com.arjuna.ats.arjuna.coordinator.*;
import com.arjuna.ats.arjuna.common.*;
import com.arjuna.mwlabs.testframework.unittest.Test;

import java.util.*;

public class Performance1 extends Test
{
    public void run(String[] args)
    {
	int numberOfTransactions = 1000;
	long stime = Calendar.getInstance().getTime().getTime();

	for (int i = 0; i < numberOfTransactions; i++)
	{
	    AtomicAction A = new AtomicAction();

	    A.begin();
	    A.abort();
	}

	long ftime = Calendar.getInstance().getTime().getTime();
	long timeTaken = ftime - stime;

	logInformation("time for "+numberOfTransactions+" write transactions is "+timeTaken);
	logInformation("number of transactions: "+numberOfTransactions);
	logInformation("throughput: "+(float) (numberOfTransactions/(timeTaken / 1000.0)));

        assertSuccess();
    }

}
