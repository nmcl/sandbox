package com.hp.mwtests.performance.servers;

import com.arjuna.mwlabs.testframework.unittest.Test;
import com.arjuna.mwlabs.testframework.unittest.LocalHarness;

import com.arjuna.orbportability.ORB;
import com.arjuna.orbportability.OA;
import com.arjuna.orbportability.common.opPropertyManager;
import com.arjuna.ats.jts.OTSManager;

import com.hp.mwtests.performance.*;

import com.hp.mwtests.performance.implementations.perftestwork.SingleResourceRegistration;
import com.hp.mwtests.performance.implementations.perftestwork.DummyPerfTestImplementation;

public class NestedOnePhaseServer extends Test
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

            SingleResourceRegistration obj1 = new SingleResourceRegistration();
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

    public static void main(String[] args)
    {
        NestedOnePhaseServer server = new NestedOnePhaseServer();

        server.initialise( null, null, args, new LocalHarness() );
        server.runTest();
    }
}
