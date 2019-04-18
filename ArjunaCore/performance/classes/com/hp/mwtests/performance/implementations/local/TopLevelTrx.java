package com.hp.mwtests.performance.implementations.local;

import com.hp.mwtests.performance.PerformanceTest;
import com.arjuna.ats.arjuna.AtomicAction;

public class TopLevelTrx extends PerformanceTest
{
    protected void work() throws Exception
    {
        try
        {
            AtomicAction tx = new AtomicAction();

            tx.begin();  // Top level begin

            if (isParameterDefined("-commit"))
                tx.commit(); // Top level commit
            else
                tx.abort(); // Top level rollback

        }
        catch (Exception e)
        {
            System.err.println("Error!!");
            e.printStackTrace();
        }
    }
}
