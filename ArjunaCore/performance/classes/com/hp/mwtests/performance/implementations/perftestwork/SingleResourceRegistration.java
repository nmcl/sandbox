package com.hp.mwtests.performance.implementations.perftestwork;

/*
 * Copyright (C) 1999-2001 by HP Bluestone Software, Inc. All rights Reserved.
 *
 * HP Arjuna Labs,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: SingleResourceRegistration.java,v 1.2 2003/06/20 15:36:14 nmcl Exp $
 */

import org.omg.CosTransactions.Control;
import org.omg.CosTransactions.Coordinator;
import org.omg.CosTransactions.Resource;
import org.omg.CosTransactions.ResourceHelper;

import com.hp.mwtests.performance.*;
import com.hp.mwtests.performance.resources.DemoResource;

import com.arjuna.ats.jts.*;
import com.arjuna.orbportability.OA;
import com.arjuna.orbportability.ORB;

public class SingleResourceRegistration extends PerfTestInterfacePOA
{
    public void work() throws PerfTestException
    {
        try
        {
            /**
             * Retrieve ORB and OA references
             */
            ORB orb = ORB.getInstance( PerformanceFramework.ORB_INSTANCE_NAME );
            OA oa = OA.getRootOA( orb );

            Control control = OTSManager.get_current().get_control();
            Coordinator coordinator = control.get_coordinator();

            DemoResource resource = new DemoResource();

            oa.objectIsReady(resource);
            Resource res = ResourceHelper.narrow(oa.corbaReference(resource));

            coordinator.register_resource(res);
        }
        catch (Exception e)
        {
            e.printStackTrace(System.err);
            throw new PerfTestException();
        }
    }
}
