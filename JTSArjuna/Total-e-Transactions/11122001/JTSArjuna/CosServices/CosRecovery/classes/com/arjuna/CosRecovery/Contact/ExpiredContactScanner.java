/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ExpiredContactScanner.java,v 1.1.2.2 2001/02/09 10:21:38 pfurniss Exp $
 */

package com.arjuna.CosRecovery.Contact;

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
 * This class is a plug-in module for the recovery manager.  This
 * class is responsible for the removing contact items that are too old
 */

public class ExpiredContactScanner implements ExpiryScanner
{
    public ExpiredContactScanner ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_CRASH_RECOVERY,
					       "ExpiredContactScanner created, with expiry time of "
					       + _expiryTime + " seconds");
	}

	_objectStore  = FactoryContactItem.getStore();
	_itemTypeName = FactoryContactItem.getTypeName();
    
    }

    /**
     * This is called periodically by the RecoveryManager
     */
    public void scan ()
    {

	// calculate the time before which items will be removed
	Date oldestSurviving = new Date( new Date().getTime() - _expiryTime * 1000);

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
				 FacilityCode.FAC_CRASH_RECOVERY,
				 "ExpiredContactScanner - scanning to remove items from before "
				 + _timeFormat.format(oldestSurviving) );
	}

	try
	{

	    InputObjectState uids = new InputObjectState();
	    
	    // find the uids of all the contact items
	    if (_objectStore.allObjUids(_itemTypeName, uids))
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
			
			FactoryContactItem anItem = FactoryContactItem.recreate(newUid);
			if (anItem != null) 
			{
			    Date timeOfDeath = anItem.getDeadTime();
			    if (timeOfDeath != null && timeOfDeath.before(oldestSurviving)) 
			    {
				if (DebugController.enabled())
				{
				    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					    FacilityCode.FAC_RECOVERY_NORMAL,
					    "Removing old contact item " + newUid);
				    
				}
				_objectStore.remove_committed(newUid, _itemTypeName);
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
    
    public boolean toBeUsed()
    {
	return _expiryTime != 0;
    }

    private String	 _itemTypeName;
    private ObjectStore _objectStore;
    private static int _expiryTime = 12 *60*60; // default is 12 hours
    private static SimpleDateFormat    _timeFormat = new SimpleDateFormat("EEE, d MMM yyyy HH:mm:ss");

    static
    {

	String expiryTimeString = PropertyManager.getProperty(RecoveryEnvironment.FACTORY_CONTACT_EXPIRY_TIME);

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
		ErrorStream.warning().println(RecoveryEnvironment.FACTORY_CONTACT_EXPIRY_TIME
								+" has inappropriate value ("
								+expiryTimeString+")");
	    }
	}
    }

};
