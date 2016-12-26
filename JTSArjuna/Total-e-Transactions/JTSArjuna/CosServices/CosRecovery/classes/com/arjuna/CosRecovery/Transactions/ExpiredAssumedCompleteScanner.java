/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ExpiredAssumedCompleteScanner.java,v 1.1.2.2 2001/02/15 21:40:09 pfurniss Exp $
 */

package com.arjuna.CosRecovery.Transactions;

import org.omg.CosTransactions.*;

import java.util.*;
import java.io.PrintWriter;
import java.text.*;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaCore.Interface.ObjectStore;
import com.arjuna.ArjunaCore.*;
import com.arjuna.ArjunaCore.Common.*;
import com.arjuna.ArjunaCore.Atomic.*;
import com.arjuna.CosTransactions.*;
import com.arjuna.CosRecovery.*;

/**
 * Implementation of {@link com.arjuna.CosRecovery.ExpiryScanner} for removing
 * relics of transactions that have been assumed complete. Instances identify the
 * particular object type to be scanned for.
 * <p>Expiry time is determined by property ASSUMED_COMPLETE_EXPIRY_TIME.
 */

public class ExpiredAssumedCompleteScanner implements ExpiryScanner
{
    private ExpiredAssumedCompleteScanner ()
    {
	// unused
    }

    protected ExpiredAssumedCompleteScanner (String typeName, ObjectStore objectStore)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_CRASH_RECOVERY,
					       "ExpiredAssumedCompleteScanner for " +
					       typeName + " created, with expiry time of "
					       + _expiryTime + " seconds");
	}

	_objectStore  = objectStore;
	_typeName = typeName;
    
    }

    public void scan ()
    {

	// calculate the time before which items will be removed
	Date oldestSurviving = new Date( new Date().getTime() - _expiryTime * 1000);

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
				 FacilityCode.FAC_CRASH_RECOVERY,
				 "ExpiredAssumedCompleteScanner - scanning to remove items from before "
				 + _timeFormat.format(oldestSurviving) );
	}

	try
	{

	    InputObjectState uids = new InputObjectState();
	    
	    // find the uids of all the contact items
	    if (_objectStore.allObjUids(_typeName, uids))
	    {
		Uid theUid = new Uid(Uid.nullUid());

		boolean endOfUids = false;

		while (!endOfUids)
		{
		    // extract a uid
		    theUid.unpack(uids);

		    if (theUid.equals(Uid.nullUid()))
			endOfUids = true;
		    else
		    {
			Uid newUid = new Uid(theUid);
			RecoveringTransaction aTransaction = null;
			if (_typeName == AssumedCompleteTransaction.typeName()) {
			    aTransaction = new AssumedCompleteTransaction(newUid);
			} else if (_typeName == AssumedCompleteServerTransaction.typeName()) {
			    aTransaction = new AssumedCompleteServerTransaction(newUid);
			} 
			// ignore imaginable logic error of it being neither
			if (aTransaction != null) 
			{
			    Date timeLastActive = aTransaction.getLastActiveTime();
			    if (timeLastActive != null && timeLastActive.before(oldestSurviving)) 
			    {
				if (DebugController.enabled())
				{
				    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					    FacilityCode.FAC_RECOVERY_NORMAL,
					    "Removing old assumed complete transaction " + newUid);
				    
				}
				_objectStore.remove_committed(newUid, _typeName);
			    }
			}
		    }
		}
	    }
	}
	catch (Exception e)
	{
	    // end of uids!
	}
    }
    /**
     * @returns false if the expiry time is zero (i.e. zero means do not expire)
     */
    public boolean toBeUsed()
    {
	return _expiryTime != 0;
    }

    private String	 _typeName;
    private ObjectStore _objectStore;
    private static int _expiryTime = 240 *60*60; // default is 240 hours
    private static SimpleDateFormat    _timeFormat = new SimpleDateFormat("EEE, d MMM yyyy HH:mm:ss");

    static
    {

	String expiryTimeString = PropertyManager.getProperty(RecoveryEnvironment.ASSUMED_COMPLETE_EXPIRY_TIME);

	if (expiryTimeString != null)
	{
	    try
	    {
		Integer expiryTimeInteger = new Integer(expiryTimeString);
		// convert to seconds
		_expiryTime = expiryTimeInteger.intValue() * 60 *60;

		if (DebugController.enabled())
		{
		    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PRIVATE,
							 FacilityCode.FAC_CRASH_RECOVERY,
							 "Expiry scan interval set to "+_expiryTime+" seconds");
		}
	    }
	    catch (NumberFormatException e)
	    {
		ErrorStream.warning().println(RecoveryEnvironment.ASSUMED_COMPLETE_EXPIRY_TIME
								+" has inappropriate value ("
								+expiryTimeString+")");
	    }
	}
    }

};
