package com.hp.mwtests.performance.implementations.local;

import com.hp.mwtests.performance.PerformanceTest;
import com.hp.mwtests.performance.records.DummyResource;
import com.arjuna.ats.arjuna.AtomicAction;

public class NestedTwoPhaseTrx extends PerformanceTest
{
    protected void work() throws Exception
    {
        try
        {
            AtomicAction tx1 = new AtomicAction();
            AtomicAction tx2 = new AtomicAction();

            tx1.begin();      // Top level
            tx2.begin();      // Nested level

            //enlist two participants for TRX
            tx1.add(DummyResource.create());
            tx2.add(DummyResource.create());

            if (isParameterDefined("-commit"))
            {
                tx2.commit();         // Nested level commit
                tx1.commit();        // Top level commit
            }
            else
            {
                tx2.abort();         // Nested level rollback
                tx1.abort();        // Top level rollback
            }
        }
        catch (Exception e)
        {
            System.err.println("Unexpected Exception: "+e);
            e.printStackTrace(System.err);
        }

    }
}
