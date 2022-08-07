/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Orbix2kRCDefaultServant.java,v 1.1.2.1.16.1 2001/04/23 11:56:00 nmcl Exp $
 */

package com.arjuna.CosRecovery.RecoveryCoordinators.orbix2000;

import org.omg.CORBA.*;
import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.CosRecovery.RecoveryCoordinators.*;
import org.omg.CosTransactions.*;

import org.omg.PortableServer.*;

public class Orbix2kRCDefaultServant extends GenericRecoveryCoordinator
{
    private ORB _ourOrb;
     
    /**
     * constructor supplies orb - used only within package
     */
     
    Orbix2kRCDefaultServant(ORB orb)
    {
	super();    // ensure id is null
	_ourOrb = orb;
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_CRASH_RECOVERY,
						 "Orbix2kRCDefaultServant(orb) ");
	}
    }
    
    public Status replay_completion ( Resource res ) throws SystemException, NotPrepared
    {
	// finding the objectid
	// get the current poa
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_CRASH_RECOVERY,
						 "Orbix2kRCDefaultServant replay_completion ");
	}
	try {
	    org.omg.CORBA.Object obj = _ourOrb.resolve_initial_references("POACurrent");
	    org.omg.PortableServer.Current poa_current = 
			    org.omg.PortableServer.CurrentHelper.narrow(obj);
	    byte[] objectId = poa_current.get_object_id();

	    String objectIdString = new String(objectId);
	    
	    // convert that to the structured id
	    RecoveryCoordinatorId  recovCoId = RecoveryCoordinatorId.reconstruct(objectIdString);
    
	    if (DebugController.enabled())
	    {
		DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						     FacilityCode.FAC_CRASH_RECOVERY,
						     "Orbix2kRCDefaultServant replay_completion for " + recovCoId );
	    }

	    // and do the real replay
	    return GenericRecoveryCoordinator.replay_completion(recovCoId, res);
	}
	catch (NotPrepared exp)
	{
	    throw exp;
	}
	catch (Exception ex)
	{
	    ErrorStream.warning().println("Orbix2kRCDefaultServant.replay_completion got " + ex);
	    ex.printStackTrace(ErrorStream.stream());
	    return Status.StatusUnknown;
	}
    }
}
