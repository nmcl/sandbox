/*
 * Copyright (C) 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Orbix2kRCManager.java,v 1.1.2.1.4.1.6.1.2.1.32.2 2001/08/07 11:11:22 nmcl Exp $
 */


package com.arjuna.CosRecovery.RecoveryCoordinators.orbix2000;

import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import org.omg.CosTransactions.*;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.OrbCommon.*;
import com.arjuna.CosRecovery.Admin.*;
import com.arjuna.CosRecovery.RecoveryCoordinators.*;

import java.io.PrintWriter;

/**
 * Implementation of RecoveryCreator for Orbix2000 3.x.
 * Handles the creation of RecoveryCoordinator objects for
 * Orbix2000 .  The RCs are created locally but also will be
 * recreated in the RecoveryManager if necessary following a crash
 * of this process.
 */

public class Orbix2kRCManager implements RcvCoManager
{

/**
 * The repository id for RecoveryCoordinator
 */
private static final String rcvcoRepositoryId = RecoveryCoordinatorHelper.id();

/**
 *  Creates RecoveryCoordinator IORs under Orbix 2000.
 *  Unlike some other RcvCoManager's, this does not create any real
 *  RecoveryCoordinator objects.
 */
public Orbix2kRCManager ()
    {
	_ourPOA = Orbix2kRCServiceInit.getRCPOA("transaction");
    }

    /**
     * We create a RecoveryCoordinator reference, but without (we think)
     * actually making the implementation object available to the orb.
     * The data needed to construct the RecoveryCoordinator is put in 
     * the ObjectId. If a replay_completion is received, it will be sent,
     * via the locationd daemon, to the RecoveryManager.
     */
    public RecoveryCoordinator makeRC( Uid RCUid, Uid tranUid,
				       Uid processUid,
				       boolean isServerTransaction )
    {
	RecoveryCoordinator rc = null;

	// mangle those parameters to the string key (object id sort of thing)

	try
	{
	    String rcObjectId = GenericRecoveryCoordinator.makeId(RCUid, tranUid, processUid, isServerTransaction);
    

	    byte[] rcObjectIdAsBytes = rcObjectId.getBytes();
	    org.omg.CORBA.Object rcAsObject = _ourPOA.create_reference_with_id(rcObjectIdAsBytes, rcvcoRepositoryId);

	    rc = RecoveryCoordinatorHelper.narrow(rcAsObject);
	
	    if (DebugController.enabled())
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						     FacilityCode.FAC_CRASH_RECOVERY,
						     "Orbix2kRCManager: "
						     +"Created reference for tran "+tranUid + " = " + rc);
	    }
	} catch (Exception ex) {
	    ErrorStream.warning().println("Orbix2kRCManager.makeRC did not make rcvco reference " 
		+ ex);
	    ex.printStackTrace(ErrorStream.stream());
	}

	return rc;
    }

public void destroy (RecoveryCoordinator rc) throws SystemException
    {
	// does nothing for Orbix 2000
    }

public void destroyAll (java.lang.Object[] params) throws SystemException
    {
	// does nothing for Orbix 2000
    }

private POA _ourPOA;

};
