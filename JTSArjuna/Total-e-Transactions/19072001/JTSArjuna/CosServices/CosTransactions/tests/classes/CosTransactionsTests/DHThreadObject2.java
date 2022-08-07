/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: DHThreadObject2.java,v 1.1 2000/02/25 14:08:46 nmcl Exp $
 */

package CosTransactionsTests;

import CosTransactionsTests.*;
import TestModule.*;
import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.CosTransactions.*;
import java.util.Random;

import java.lang.InterruptedException;

public class DHThreadObject2 extends Thread
{

public DHThreadObject2 (char c)
    {
        chr = c;
    }

public void run ()
    {
        for (int i = 0; i < 100; i++)
        {
            DistributedHammerWorker2.randomOperation(chr, 0);
            Util.highProbYield();
        }
    }
    
private char chr;
    
};
