/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Visi3RCActivator.java,v 1.1.2.2 2000/07/30 20:26:56 ndbi Exp $
 */

package com.arjuna.CosRecovery.Activators.vbroker;

import org.omg.CORBA.*;
import com.visigenic.vbroker.extension.*;
import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.OrbCommon.*;
import com.arjuna.CosRecovery.Coordinators.vbroker.*;

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
	    System.out.println("com.arjuna.CosRecovery.Activators.vbroker.Visi3RCActivator created");
	}
    }

    public org.omg.CORBA.Object activate(ImplementationDef impl) 
    {
	if (DebugController.enabled()) 
	{
	    System.out.println("Activator called " + ++_count + " times");
	}
	
	byte[] ref_data = ((ActivationImplDef) impl).id();
	String encodedRCData = new String(ref_data, 0, ref_data.length-1);
	Visi3RecoveryCoordinator obj = new Visi3RecoveryCoordinator(encodedRCData);
	ORBInterface.boa().obj_is_ready(obj);
	return obj;
    }

    public void deactivate(org.omg.CORBA.Object obj, ImplementationDef impl) 
    {
	// nothing to do here...
    }

    private static int _count;

};


