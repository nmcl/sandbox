package com.hp.mwtests.performance.implementations.perftestwork;

import com.hp.mwtests.performance.PerfTestInterfacePOA;
import com.hp.mwtests.performance.PerfTestException;
import com.hp.mwtests.performance.PerformanceFramework;
import com.hp.mwtests.performance.resources.DemoSubTranResource;
import com.arjuna.ats.jts.OTSManager;
import com.arjuna.orbportability.ORB;
import com.arjuna.orbportability.OA;
import org.omg.CosTransactions.SubtransactionAwareResourceHelper;
import org.omg.CosTransactions.SubtransactionAwareResource;
import org.omg.CosTransactions.Control;
import org.omg.CosTransactions.Coordinator;

public class SingleSubtranAwareResourceRegistration extends PerfTestInterfacePOA
{
    public void work() throws PerfTestException
    {
        try
        {
            /**
             * Retrieve ORB and OA references
             */
            ORB orb = ORB.getInstance(PerformanceFramework.ORB_INSTANCE_NAME);
            OA oa = OA.getRootOA(orb);

            Control control = OTSManager.get_current().get_control();
            Coordinator coordinator = control.get_coordinator();

            DemoSubTranResource resource = new DemoSubTranResource();

            oa.objectIsReady(resource);
            SubtransactionAwareResource res = SubtransactionAwareResourceHelper.narrow(oa.corbaReference(resource));
            coordinator.register_resource(res);
        }
        catch (Exception e)
        {
            throw new PerfTestException();
        }
    }
}
