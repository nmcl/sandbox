/*
 * Copyright (C) 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: Orbix2kRCServiceInit.java,v 1.1.2.1.4.1.8.1.30.2 2001/08/07 11:11:22 nmcl Exp $
 */

package com.arjuna.CosRecovery.RecoveryCoordinators.orbix2000;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.OrbCommon.*;
import com.arjuna.CosTransactions.OTS_RecoveryCreator;
import com.arjuna.CosRecovery.RecoveryCoordinators.*;

import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import org.omg.CosTransactions.*;

import com.iona.corba.IT_CORBA.*;
import com.iona.corba.IT_PortableServer.*;
import com.iona.corba.IT_CORBA.WELL_KNOWN_ADDRESSING_POLICY_ID;
import com.iona.corba.IT_PortableServer.PERSISTENCE_MODE_POLICY_ID;
import com.iona.corba.IT_PortableServer.PersistenceModePolicyValue;

/**
 * Initialises Orbix2000 RecoveryCoordinator creation subsystem
 * and provides the Orbix-specific implementations of stuff
 *
 * All orbs are likely to be the same, constructing a GenericRecoveryCreator,
 * but with an orb-specific manager
 */
public class Orbix2kRCServiceInit 
{
	
private Orbix2kRCServiceInit()
{
}

/**
 * Provide the POA for the recoverycoordinator.
 * Construct with the policies appropriate for its use in the RecoveryManager,
 * but the policies are usable by the Orbix2kRCManager to create the IOR's in
 * TS-using processes.
 */
 
static POA getRCPOA (String domainName)
{
    String rcServiceName = GenericRecoveryCreator.getRecCoordServiceName();
    
    if (DebugController.enabled())
    {
	DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
			 FacilityCode.FAC_CRASH_RECOVERY,
			 "Orbix2kRCServiceInit.getRCPOA " + rcServiceName);
    }

    if (_poa == null) {
	String poaName = POA_NAME_PREFIX + GenericRecoveryCreator.getRecCoordServiceName();
    
	// organise the POA
	
	// get the orb and the rootPOA from Orb portability

	org.omg.CORBA.ORB theOrb = ORBInterface.orb();
	POA root_poa = OAInterface.rootPoa();

	// construct the Orbix-proprietary policies

	/*
	 * Start with the well-known addressing policy, so we can do lazy
	 * RecoveryCoordinator creation properly.
	 */

	try {
	    Any wellKnownAddressingPolicy = theOrb.create_any();
	    
	    wellKnownAddressingPolicy.insert_string(domainName);

	    // specify direct persistence

	    Any persistenceValue = theOrb.create_any();
	    PersistenceModePolicyValueHelper.insert(persistenceValue, PersistenceModePolicyValue.DIRECT_PERSISTENCE);

	    // create direct persistent POA

	    // make the policy lists, with standard policies

	    org.omg.CORBA.Policy[] policies = null;
	    
	    policies = new Policy []
	    {
		root_poa.create_lifespan_policy(LifespanPolicyValue.PERSISTENT),
		root_poa.create_thread_policy(ThreadPolicyValue.ORB_CTRL_MODEL),
		root_poa.create_id_assignment_policy(IdAssignmentPolicyValue.USER_ID),
		root_poa.create_id_uniqueness_policy(IdUniquenessPolicyValue.MULTIPLE_ID),
		root_poa.create_request_processing_policy(RequestProcessingPolicyValue.USE_DEFAULT_SERVANT),
		theOrb.create_policy(WELL_KNOWN_ADDRESSING_POLICY_ID.value, wellKnownAddressingPolicy),
		theOrb.create_policy(PERSISTENCE_MODE_POLICY_ID.value, persistenceValue)
	    };

	    _poa = root_poa.create_POA(poaName, null, policies);
	}
	catch (Exception ex)
	{
	    ErrorStream.warning().println("Failed to create poa for recoverycoordinators " + ex);
	    ex.printStackTrace(ErrorStream.stream());
	}
    }

    return _poa;    
}

/**
 * This starts the service in the RecoveryManager.
 */

public static boolean startRCservice ()
{
    POA ourPOA = getRCPOA("recovery_coordinator");

    try {
	// get the orb, so we can pass it to the default servant
	
	org.omg.CORBA.ORB theOrb = ORBInterface.orb();
	
	// make the default servant
	Orbix2kRCDefaultServant theButler = new Orbix2kRCDefaultServant(theOrb);    
    
	// register it on the POA
	ourPOA.set_servant(theButler);

	// activate the poa 
	ourPOA.the_POAManager().activate();

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
			     FacilityCode.FAC_CRASH_RECOVERY,
			     "Orbix2kRCServiceInit - set default servant and activated");
	}

	return true;
    } catch (Exception ex) {
	ErrorStream.warning().println("Failed to start RC service " + ex);
	ex.printStackTrace(ErrorStream.stream());
	return false;
    }    
    
}

private static final String POA_NAME_PREFIX = "RcvCo-";

private static POA _poa = null;

};


