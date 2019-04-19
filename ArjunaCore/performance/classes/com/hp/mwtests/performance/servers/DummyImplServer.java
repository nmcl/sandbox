package com.hp.mwtests.performance.servers;

import com.arjuna.mwlabs.testframework.unittest.Test;
import com.arjuna.orbportability.ORB;
import com.arjuna.orbportability.OA;
import com.arjuna.ats.jts.OTSManager;
import com.hp.mwtests.performance.PerformanceFramework;
import com.hp.mwtests.performance.PerfTestInterface;
import com.hp.mwtests.performance.PerfTestInterfaceHelper;
import com.hp.mwtests.performance.implementations.perftestwork.SingleResourceRegistration;
import com.hp.mwtests.performance.implementations.perftestwork.DummyPerfTestImplementation;

public class DummyImplServer extends Test
{
    /**
     * The main test method which must assert either a pass or a fail.
     */
    public void run(String[] args)
    {
        try
        {
            String serviceName = args[ args.length - 2 ];
            String serviceName2 = args[ args.length - 1 ];

            /**
             * Retrieve ORB and OA references, intialise them
             * and then set the OTSManager ORB and OA properties
             */
            ORB orb = ORB.getInstance( PerformanceFramework.ORB_INSTANCE_NAME );
            OA oa = OA.getRootOA( orb );

            orb.initORB(args, null);
            oa.initOA(args);

            OTSManager.setORB(orb);
            OTSManager.setPOA(oa);

            DummyPerfTestImplementation obj1 = new DummyPerfTestImplementation();
            oa.objectIsReady(obj1);
            PerfTestInterface objRef1 = PerfTestInterfaceHelper.narrow(oa.corbaReference(obj1));

            registerService( serviceName, orb.orb().object_to_string(objRef1) );

            DummyPerfTestImplementation obj2 = new DummyPerfTestImplementation();
            oa.objectIsReady(obj2);
            PerfTestInterface objRef2 = PerfTestInterfaceHelper.narrow(oa.corbaReference(obj2));

            registerService( serviceName2, orb.orb().object_to_string(objRef2) );

            assertSuccess();
            assertReady();

            orb.orb().run();
        }
        catch (Exception e)
        {
            e.printStackTrace(System.err);
            assertFailure();
        }
    }
}
