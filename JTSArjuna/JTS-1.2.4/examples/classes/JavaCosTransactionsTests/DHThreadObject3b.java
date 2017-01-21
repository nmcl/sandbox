/*
 * Copyright (C) 1996, 1997, 1998, 1999,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DHThreadObject3b.java,v 1.2.6.1 1999/02/22 11:10:10 nmcl Exp $
 */

package JavaCosTransactionsTests;

import TestModule.*;
import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.JavaCosTransactions.*;
import java.util.Random;

import JavaCosTransactionsTests.TestException;
import java.lang.InterruptedException;

public class DHThreadObject3b extends Thread
{

public DHThreadObject3b (int id)
    {
        _id = id;
    }

public void run ()
    {
        for (int i = 0; i < 10; i++)
        {
            DistributedHammerWorker3.randomOperation(_id, 0);
            Util.highProbYield();
        }
    }

private int _id;
    
};
