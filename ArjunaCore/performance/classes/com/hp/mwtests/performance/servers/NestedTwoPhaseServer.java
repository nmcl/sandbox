package com.hp.mwtests.performance.servers;

import com.arjuna.mwlabs.testframework.unittest.Test;
import com.arjuna.orbportability.ORB;
import com.arjuna.orbportability.OA;
import com.hp.mwtests.performance.PerformanceFramework;
import com.hp.mwtests.performance.PerfTestInterface;
import com.hp.mwtests.performance.PerfTestInterfaceHelper;
import com.hp.mwtests.performance.implementations.perftestwork.SingleResourceRegistration;
import com.hp.mwtests.performance.implementations.perftestwork.SingleSubtranAwareResourceRegistration;

public class NestedTwoPhaseServer extends Test
{
    /**
     * The main test method which must assert either a pass or a fail.
     */
    public void run(String[] args)
    {
        try
        {
            String serviceName = args[args.length - 2];
            String serviceName2 = args[args.length - 1];

            /**
             * Retrieve ORB and OA references, intialise them
             * and then set the OTSManager ORB and OA properties
             */
            ORB orb = ORB.getInstance(PerformanceFramework.ORB_INSTANCE_NAME);
            OA oa = OA.getRootOA(orb);

            orb.initORB(args, null);
            oa.initOA(args);

            SingleResourceRegistration obj1 = new SingleResourceRegistration();
            oa.objectIsReady(obj1);
            PerfTestInterface objRef1 = PerfTestInterfaceHelper.narrow(oa.corbaReference(obj1));

            registerService(serviceName, orb.orb().object_to_string(objRef1) );

            SingleSubtranAwareResourceRegistration obj2 = new SingleSubtranAwareResourceRegistration();
            oa.objectIsReady(obj2);
            PerfTestInterface objRef2 = PerfTestInterfaceHelper.narrow(oa.corbaReference(obj2));

            registerService(serviceName2, orb.orb().object_to_string(objRef2) );

            System.out.println("Object reference written to file");

            assertSuccess();
            assertReady();

            orb.orb().run();
        }
        catch (Exception e)
        {
            System.err.println("Unexpected exception: "+e);
            e.printStackTrace(System.err);

            assertFailure();
        }
    }
}
