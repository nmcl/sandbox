/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Visi3RCActivator.java,v 1.1.2.1.2.1 2001/01/31 12:12:21 pfurniss Exp $
 */

package com.arjuna.CosRecovery.RecoveryCoordinators.vbroker3;

import org.omg.CORBA.*;
import com.visigenic.vbroker.extension.*;
import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.OrbCommon.*;
import com.arjuna.CosRecovery.RecoveryCoordinators.*;

/**
 * A Visibroker Activator that is used to dynamically create
 * RecoveryCoordinator objects on demand.
 */

public class Visi3RCActivator implements Activator 
{
    public Visi3RCActivator()
    {
	if (DebugController.enabled()) 
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_CRASH_RECOVERY,
						 "Visi3RCActivator created");
	}
    }

    public org.omg.CORBA.Object activate(ImplementationDef impl) 
    {
	if (DebugController.enabled()) 
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_CRASH_RECOVERY,
						 "Visi3RCActivator called " + ++_count + " times");
	}
	
	byte[] ref_data = ((ActivationImplDef) impl).id();
	String encodedRCData = new String(ref_data, 0, ref_data.length);
	GenericRecoveryCoordinator obj = GenericRecoveryCoordinator.reconstruct(encodedRCData);
	if (obj != null) {
	    OAInterface.boa().obj_is_ready(obj);
	}
	return obj;
    }

    public void deactivate(org.omg.CORBA.Object obj, ImplementationDef impl) 
    {
	// nothing to do here...
    }

    private static int _count;

};


