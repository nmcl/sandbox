/*
 * Copyright (C) 2001
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: RecoveryContactWriter.java,v 1.1.2.2 2001/01/19 10:07:52 pfurniss Exp $
 */

package com.arjuna.CosRecovery.Contact;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaOTS.*;
import com.arjuna.OrbCommon.event.*;

/**
 * An instance of this object is registered so as to be invoked when any 
 * CORBA object is constructed, but ignores everything except the first ArjunaFactory
 *
 * On seeing an ArjunaFactory, it causes a FactoryContactItem to be saved and
 * deregisters itself
 * (and thus relies on the fact (true for 2.1) that any ArjunaFactory can be used to 
 * find the status of any transaction.
 *
 */

public class RecoveryContactWriter implements com.arjuna.OrbCommon.event.EventHandler
{

private boolean _noted;

public RecoveryContactWriter()
{
    if (DebugController.enabled())
    {
	DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
					     FacilityCode.FAC_CRASH_RECOVERY,
					     "RecoveryContactWriter() created");
    }
    _noted = false;
}

public void connected (org.omg.CORBA.Object obj)
{    
    if (DebugController.enabled())
    {
	DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					     FacilityCode.FAC_CRASH_RECOVERY,
					     "RecoveryContactWriter.connected( " + obj + ")");
	}
    // only do this once - but shouldn't need this, since de-register on writing
    if (_noted) {
	return;
    }
    try {
	ArjunaFactory theFactory = ArjunaFactoryHelper.narrow(obj);
	
	if (theFactory != null) {
	    // if that didn't blow, we have a hit
	    if (DebugController.enabled())
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						     FacilityCode.FAC_CRASH_RECOVERY,
						     "RecoveryContactWriter.connected - found ArjunaFactory");
	    }
	    FactoryContactItem.createAndSave(theFactory);
	    // we've done our work, so set the flag and try to remove ourselves
	    _noted = true;
	    EventManager.getManager().removeHandler(this);
	}
    } catch ( Exception ex) {
	// oh well - it probably wasn't ours
    }
}

public void disconnected (org.omg.CORBA.Object obj)
{
    // nothing to be done
}

public String name ()
{
    return "RecoveryContactEventHandler";
}


}




