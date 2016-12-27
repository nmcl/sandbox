/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ThreadObject2.java,v 1.2 1998/07/06 13:29:49 nmcl Exp $
 */

package JavaCosTransactionsTests;

import TestModule.*;
import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.JavaCosTransactions.*;
import java.util.Random;

import java.lang.InterruptedException;

public class ThreadObject2 extends Thread
{

public ThreadObject2 (char c)
    {
        chr = c;
    }

public void run ()
    {
        for (int i = 0; i < 100; i++)
        {
            AtomicWorker2.randomOperation(chr, 0);
            Util.highProbYield();
        }
    }
    
private char chr;
    
};
