package com.hp.mwtests.performance.implementations.remote.nestedonephase;

import com.hp.mwtests.performance.PerformanceTest;
import com.hp.mwtests.performance.PerformanceFramework;
import com.hp.mwtests.performance.PerfTestInterface;
import com.hp.mwtests.performance.PerfTestInterfaceHelper;
import com.arjuna.ats.jts.OTSManager;
import com.arjuna.orbportability.ORB;
import com.arjuna.orbportability.OA;
import org.omg.CORBA.Current;

public class NestedOnePhaseTrx extends PerformanceTest
{
    protected void work() throws Exception
    {
        ORB orb = ORB.getInstance( PerformanceFramework.ORB_INSTANCE_NAME );
        OA oa = OA.getRootOA(orb);

        String ref1 = getServiceConfig(0);
        String ref2 = getServiceConfig(1);

        org.omg.CORBA.Object obj1 = orb.orb().string_to_object(ref1);
        PerfTestInterface d1 = (PerfTestInterface) PerfTestInterfaceHelper.narrow(obj1);

        OTSManager.get_current().begin();
        d1.work();

        org.omg.CORBA.Object obj2 = orb.orb().string_to_object(ref2);
        PerfTestInterface d2 = (PerfTestInterface) PerfTestInterfaceHelper.narrow(obj2);

        OTSManager.get_current().begin();
        d2.work();

        // nested commit
        OTSManager.get_current().commit(true);

        if ( true )
        {
            // top level commit
            OTSManager.get_current().commit(true);
        }
        else
        {
             // top level rollback
            OTSManager.get_current().rollback();
        }
    }
}
