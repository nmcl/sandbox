package com.hp.mwtests.performance.implementations.local;

// Top-level transactions with two enlisted participants.
// (two phase commit/rollback)

import com.arjuna.ats.arjuna.AtomicAction;
import com.arjuna.ats.arjuna.coordinator.*;
import com.arjuna.ats.arjuna.common.*;

import com.hp.mwtests.performance.PerformanceTest;
import com.hp.mwtests.performance.records.DummyResource;


public class TwoPhaseTrx extends PerformanceTest
{
    public void work()
    {
        try
        {
            AtomicAction tx = new AtomicAction();
            tx.begin(); // Top level begin

            // enlist two participants (resource) two-phase commit

            tx.add(DummyResource.create());
            tx.add(DummyResource.create());

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
