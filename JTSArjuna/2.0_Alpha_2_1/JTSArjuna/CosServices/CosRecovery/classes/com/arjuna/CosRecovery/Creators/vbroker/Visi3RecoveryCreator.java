/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Visi3RecoveryCreator.java,v 1.1.2.2 2000/07/30 20:26:56 ndbi Exp $
 */

package com.arjuna.CosRecovery.Creators.vbroker;

import org.omg.CosTransactions.*;
import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaLite.Atomic.*;
import com.arjuna.OrbCommon.*;
import com.arjuna.CosTransactions.*;
import com.arjuna.ArjunaOTS.*;

import com.arjuna.CosRecovery.Coordinators.vbroker.Visi3RecoveryCoordinator;

/**
 * Implementation of RecoveryCreator for Visibroker 3.x.
 */

public class Visi3RecoveryCreator extends OTS_RecoveryCreator
{
    /**
     * Handles the creation of RecoveryCoordinator objects for
     * Visibroker 3.x.  The RCs are created locally but also will be
     * recreated in the RecoveryManager if necessary following a crash
     * of this process.
     */

    public RecoveryCoordinator create (Resource res, Object[] params) 
    {
	RecoveryCoordinator recoveryCoordinator = null;
	
	if (DebugController.enabled()) 
	{
	    System.out.print("com.arjuna.CosRecovery.Creators.vbroker.Visi3RecoveryCreator");
	    if (res != null)
		System.out.println(".create("+res+")");
	    else
		System.out.println(".create()");
	}

	if ((res != null) && (params != null) && (params[0] != null) )
	{
	    int index = 0;

	    OTS_Transaction otsTransaction = (OTS_Transaction) params[index++];

	    // OTS_Transaction is a TIE. We need to get the Control object for a CORBA reference.
	    // We need the Control for the top-level transaction.
	    OTS_Control topLevelControl = otsTransaction.getControlHandle();
	    while (topLevelControl.getParentControl() != null)
		topLevelControl = (OTS_Control) topLevelControl.getParentControl();

	    // Get the Uid of the top-level transaction
	    BasicAction rootAction = otsTransaction;
	    while ((rootAction.parent()) != null)
		rootAction = rootAction.parent();
	    Uid rootActionUid = rootAction.get_uid();
	    
	    // Create a Uid for the new RecoveryCoordinator
	    Uid RCUid = new Uid();
	    
	    // Now make the RecoveryCoordinator
	    recoveryCoordinator = makeRC (RCUid, rootActionUid, res, topLevelControl);
	    
	    // Tidy up
	    otsTransaction = null;
	    topLevelControl = null;
	    rootAction = null;

	    // Pass the RecoveryCoordinator Uid back
	    params[0] = RCUid;
	}
	else
	{
	    System.err.println("com.arjuna.CosRecovery.Creators.vbroker.Visi3RecoveryCreator"
			       +": Missing params to create");
	}
	return recoveryCoordinator;
    }

    public RecoveryCoordinator makeRC( Uid rcUid, Uid tranUid, Resource res, OTS_Control con )
    {
	/**
	 * We create the RecoveryCoordinator locally but also link the
	 * reference with the service name that is handled by the
	 * Visi3RCActivator. The data needed to construct the
	 * RecoveryCoordinator is also packed in to the reference
	 * data. If this process crashes then the RecoveryCoordinator
	 * will be recreated by the Activator in the recovery daemon
	 * if a request is made. Pretty cool!
	 */

	RecoveryCoordinator rc = null;
	String rcObjectKey = null;
	
	// Pack the parameters in to the object key.
	// TODO - replace ':' with '-' if required

	try 
	{
	    rcObjectKey = rcUid.toString() + _RecCoordServiceObjKeyDelimiter +
		tranUid.toString() + _RecCoordServiceObjKeyDelimiter +
		ORBInterface.orb().object_to_string(res) + _RecCoordServiceObjKeyDelimiter +
		ORBInterface.orb().object_to_string(con);
	}
	catch (Exception e)
	{
	    System.err.println("System exception when creating RecoveryCoordinator object key "+e);
	}
	
	// Now create the implementation object
	rc = new Visi3RecoveryCoordinator(rcUid, tranUid, res, con);

	// Encode the Object Key in the reference data
	int length = rcObjectKey.length(); 
	byte[] ref_data = new byte[length];
	//	rcObjectKey.getBytes(0, length, ref_data, 0); - deprecated
	ref_data = rcObjectKey.getBytes();

	// Ready the object with the ServiceName and reference data
	ORBInterface.boa().obj_is_ready(rc, _RecCoordServiceName, ref_data);

	return rc;
    }
    
    public static String getRecCoordServiceName ()
    {
	return _RecCoordServiceName;
    }

    public static char getRecCoordServiceObjKeyDelimiter ()
    {
	return _RecCoordServiceObjKeyDelimiter;
    }

    public RecoveryCoordinator makeRCIOR( String host, String objKey )
    {
	/** 
	 * NOT USED! - for reference only
	 * 
	 * Another approach for deferring the creation of recovery coordinators.
	 * This uses a defered bind.
	 */
	if (DebugController.enabled())
	{
	    System.out.println("com.arjuna.CosRecovery.Creators.vbroker.Visi3RecoveryCoordinator"
 			       +".makeRCIOR("+host+", "+objKey+")");
	}

	// Get the bind options
	org.omg.CORBA.BindOptions bindOptions = new org.omg.CORBA.BindOptions();

	// Cache the current bind options
	boolean oldDeferBindOption = bindOptions.defer_bind;

	// Set to defer bind
	bindOptions.defer_bind = true;

	// Bind to the as yet uncreated RecoveryCoordinator
	RecoveryCoordinator theRecoveryCoordinator = 
	    RecoveryCoordinatorHelper.bind(ORBInterface.orb(), objKey, host, bindOptions);

	// Reset the bind options
	bindOptions.defer_bind = oldDeferBindOption;

	System.out.println("The new RC = "+theRecoveryCoordinator);

	return theRecoveryCoordinator;
    }
    
    static final char _RecCoordServiceObjKeyDelimiter = '*';
    static final String _RecCoordServiceName = "RecCoService";
};






