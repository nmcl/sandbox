/*
 * Copyright (C) 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Orbix2kRecMan.javatmpl,v 1.1.4.1 2001/03/19 15:53:03 nmcl Exp $
 */
 
/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OrbPortability.h,v 1.1 2000/02/25 14:09:59 nmcl Exp $
 */




/*
 * Try to get around the differences between Ansi CPP and
 * K&R cpp with concatenation.
 */


/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: OrbPortability_stdc.h,v 1.2.4.1.2.3.26.1.2.2.4.2 2001/02/02 11:51:52 nmcl Exp $
 */
































































package com.arjuna.CosRecovery.RecoveryCoordinators.orbix2000;

import com.arjuna.OrbCommon.*;
import org.omg.CosTransactions.*;
import com.arjuna.CosRecovery.*;
import com.arjuna.ArjunaCommon.Common.*;
import org.omg.PortableServer.*;
import org.omg.CORBA.Policy;

import org.omg.CORBA.BAD_OPERATION;
import org.omg.CORBA.SystemException;

public class Orbix2kRecMan extends  com.arjuna.CosRecovery.Admin . RecoveryManagerPOA 
{

public Orbix2kRecMan ()
    {
	_ourPOA = Orbix2kRCServiceInit.getRCPOA();

	if (DebugController.enabled()) 
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_CRASH_RECOVERY,
						 "Orbix2kRecMan created");
	}
	
	OAInterface.objectIsReady( this ) ;
	
	com.arjuna.CosRecovery.Admin.RecoveryManager theMan =  com.arjuna.CosRecovery.Admin.RecoveryManagerHelper.narrow(OAInterface.corbaReference( this )) ;

	try
	{
	    ORBServices.registerService(theMan, RecManName, null, ORBServices.CONFIGURATION_FILE);
	}
	catch (Exception e)
	{
	    ErrorStream.fatal().println("Orbix2kRecMan registerService failed: "+e);
	    
	    throw new FatalError(e.toString());
	}
    }

public String createIOR (String id) throws SystemException
    {
	try
	{
	    org.omg.CORBA.Object rcAsObject = _ourPOA.create_reference_with_id(id.getBytes(), rcvcoRepositoryId);

	    return ORBInterface.orb().object_to_string(rcAsObject);
	}
	catch (Exception e)
	{
	    ErrorStream.warning().println("Orbix2kRecMan.createIOR - caught: "+e);

	    throw new BAD_OPERATION(e.toString());
	}
    }

public static final String RecManName = "RecoveryManager";

private POA _ourPOA;

private static final String rcvcoRepositoryId = RecoveryCoordinatorHelper.id();

}

