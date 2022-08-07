/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: DHThreadObject3b.java,v 1.1 2000/02/25 14:08:47 nmcl Exp $
 */

package CosTransactionsTests;

import TestModule.*;
import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.CosTransactions.*;
import java.util.Random;

import CosTransactionsTests.TestException;
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
