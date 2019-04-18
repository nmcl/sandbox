package com.hp.mwtests.performance.implementations.local;

import com.hp.mwtests.performance.PerformanceTest;
import com.hp.mwtests.performance.records.DummyResource;
import com.arjuna.ats.arjuna.AtomicAction;

public class OnePhaseTrx extends PerformanceTest
{
    protected void work() throws Exception
    {
        try
        {
            AtomicAction tx = new AtomicAction();
            tx.begin();   // Top level begin

            //enlist the single participant (resource) one-phase commit
            DummyResource ds = new DummyResource();
            tx.add(ds);

            if (isParameterDefined("-commit"))
                tx.commit();  // Top level commit
            else
                tx.abort();  // Top level rollback

        }
        catch (Exception e)
        {
            System.err.println("Unexpected Exception: "+e);
            e.printStackTrace(System.err);
        }
    }
}
