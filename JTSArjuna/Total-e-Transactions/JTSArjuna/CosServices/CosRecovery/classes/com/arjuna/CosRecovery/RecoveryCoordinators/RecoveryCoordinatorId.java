/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: RecoveryCoordinatorId.java,v 1.1.2.1.16.1 2001/04/27 09:04:10 nmcl Exp $
 */

package com.arjuna.CosRecovery.RecoveryCoordinators;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaCore.Common.*;
import com.arjuna.ArjunaCore.Interface.ObjectStore;
import com.arjuna.OrbCommon.*;
import com.arjuna.CosRecovery.*;
import com.arjuna.CosRecovery.Contact.StatusChecker;
import com.arjuna.CosRecovery.Transactions.*;
import com.arjuna.CosTransactions.OTS_Transaction;
import com.arjuna.CosTransactions.Interposition.OTS_ServerTransaction;

import org.omg.CosTransactions.*;

import org.omg.CORBA.SystemException;
import org.omg.CORBA.BAD_PARAM;
import org.omg.CORBA.UNKNOWN;

/**
 * Holds and manipulates the fields used to identify which branch of which transaction
 * a RecoveryCoordinator is concerned with. Split from {@link GenericRecoveryCoordinator} to
 * allow for default servant mechanism with POA
 *
 * @author Peter Furniss (peter.furniss@arjuna.com)
 * @version $Id: RecoveryCoordinatorId.java,v 1.1.2.1.16.1 2001/04/27 09:04:10 nmcl Exp $ 
*/

public class RecoveryCoordinatorId
{
    /* fields have package access - we are close friends with GenericRecoveryCoordinator */
    Uid	      _RCUid;
    Uid	      _actionUid;
    Uid	    _originalProcessUid;
    boolean     _isServerTransaction;

    /**
     * Constructor with separate fields
     */
    RecoveryCoordinatorId (Uid RCUid, Uid actionUid, 
				       Uid processUid, boolean isServerTransaction)
    {
	_RCUid = RCUid;
	_actionUid = actionUid;
	_originalProcessUid = processUid;
	_isServerTransaction = isServerTransaction;
    }
    
	/**
     * Construct a string, to be used somehow in the objectkey (probably)
     * of a RecoveryCoordinator reference. This will be deconstructed in 
     * the reconstruct() which is passed such a string, to remake the
     * necessary RecoveryCoordinator when a replay_completion is received for it.
     *
     * Put here to make it in the same class as the deconstruction
     */
    String makeId()
    {
	String rcObjectKey = null;

	/*
	 * Pack the fields in to the string.
	 * perhaps replace ':' with '-' if required
	 *   (likely to be orb-specific requirement)
	 */

	try
	{
	    StringBuffer stringBuf = new StringBuffer();
	    stringBuf.append(_RCUid.toString());
	    stringBuf.append(_ObjKeyDelimiter);
	    stringBuf.append(_actionUid.toString());
	    stringBuf.append(_ObjKeyDelimiter);
	    stringBuf.append(_originalProcessUid.toString());
	    stringBuf.append(_ObjKeyDelimiter);
	    stringBuf.append(_isServerTransaction);
	    rcObjectKey = stringBuf.toString();

	    if (DebugController.enabled())
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
			    FacilityCode.FAC_CRASH_RECOVERY,
			    "RecoveryCoordinatorId: created RCkey "+rcObjectKey );
	    }
	}
	catch (Exception e)
	{
	    ErrorStream.warning().println("System exception when creating RecoveryCoordinator object key "
					  +e);
	}
	return rcObjectKey;
    }

    /**
     *  Construct an id from the encoded string
     * @returns null if parse fails 
     */
    public static RecoveryCoordinatorId reconstruct(String encodedRCData)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
			 FacilityCode.FAC_CRASH_RECOVERY,
			 "RecoveryCoordinatorId(" + encodedRCData + ")");
	}
	Uid	      RCUid = null;
	Uid	      actionUid = null;
	Uid	    originalProcessUid = null;
	boolean     isServerTransaction = false;

	boolean ok = (encodedRCData != null);

	if (ok)
	{
	    int index1 = encodedRCData.indexOf(_ObjKeyDelimiter);
	    int index2 = 0;
	    
	    if (index1 != -1)
	    {
		String stringifiedRCUid = encodedRCData.substring(0, index1);
		RCUid = new Uid (stringifiedRCUid);
	    }
	    else
		ok = false;
	    
	    if (ok)
	    {
		index2 = encodedRCData.indexOf(_ObjKeyDelimiter, index1 +1);
		
		if (index2 != -1)
		{
		    String stringifiedTranUid = encodedRCData.substring(index1 +1, index2);
		    actionUid = new Uid (stringifiedTranUid);
		    index1 = index2;
		}
		else
		    ok = false;
	    }
	    
	    if (ok)
	    {
		index2 = encodedRCData.indexOf(_ObjKeyDelimiter, index1 +1);
		
		if (index2 != -1)
		{
		    String stringifiedProcessUid = encodedRCData.substring(index1 +1, index2);
		    originalProcessUid = new Uid (stringifiedProcessUid);
		    index1 = index2;
		}
		else
		    ok = false;
	    }

	    
	    if (ok)
	    {
		String stringifiedIsServerTransaction = encodedRCData.substring(index1 +1);
		isServerTransaction = (Boolean.valueOf(stringifiedIsServerTransaction)).booleanValue();
	    }
	}
	
	if (ok) {
	    return new RecoveryCoordinatorId (RCUid, actionUid,
				 originalProcessUid, isServerTransaction);
	} else {
	    ErrorStream.warning().println("RecoveryCoordinatorId could not decode data " + encodedRCData);
	    return null;
	}
    }

    /**
     * override base toString for clarity
     */
    public String toString()
    {
	return "(" + _RCUid+", "+_actionUid+", " + _originalProcessUid 
			 + (_isServerTransaction ? ", interposed-tx" : ", root-tx" ) + ")" ;

    }
    

    private static final char   _ObjKeyDelimiter = '*';

}


