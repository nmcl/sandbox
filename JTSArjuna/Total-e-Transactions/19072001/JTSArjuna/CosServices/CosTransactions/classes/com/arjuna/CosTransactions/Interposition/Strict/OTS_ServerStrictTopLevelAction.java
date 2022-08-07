/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_ServerStrictTopLevelAction.javatmpl,v 1.1.8.2.8.1.14.1 2000/12/13 15:40:26 nmcl Exp $
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
































































package com.arjuna.CosTransactions.Interposition.Strict;

import com.arjuna.ArjunaCommon.Template.*;
import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.CosTransactions.*;
import com.arjuna.CosTransactions.Interposition.OTS_ServerControl;
import com.arjuna.CosTransactions.Interposition.Arjuna.*;
import com.arjuna.OrbCommon.*;
import org.omg.CosTransactions.*;
import com.arjuna.ArjunaOTS.*;

public class OTS_ServerStrictTopLevelAction extends OTS_ServerTopLevelAction
{
    /*
     * The base class is responsible for registering with
     * the parent transaction.
     */
    
public OTS_ServerStrictTopLevelAction (OTS_ServerControl control, boolean doRegister)
    {
	super(control);

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OTS, "OTS_ServerStrictTopLevelAction::OTS_ServerStrictTopLevelAction (OTS_ServerControl, "+doRegister+" )");
	}

	_registered = false;
	_theResource = null;
	
	if (_theControl != null)
	{
	    _theResource = new  org.omg.CosTransactions. ResourcePOATie (this);
	    
	    OAInterface.objectIsReady( _theResource ) ;
	    
	    /*
	     * Would like to be able to attach a thread filter
	     * to this object if process-filters aren't supported.
	     * However, currently this won't work as we can't have
	     * two different filter types working at the same
	     * time.
	     *
	     *		ATTACH_THREAD_FILTER_(_theResource);
	     */
	    
	    if (doRegister)
		interposeResource();
	}
    }

public boolean interposeResource ()
    {
	if (!_registered)
	{
	    _registered = true;

	    if ((_theResource != null) && (_theControl != null))
	    {
		Coordinator realCoordinator = _theControl.originalCoordinator();

		if (!(_valid = registerResource(realCoordinator)))
		{
		    ErrorStream.stream(ErrorStream.WARNING).println("OTS_ServerStrictTopLevelAction - could not register interposed hierarchy!");

		    /*
		     * Failed to register. Valid is set, and the interposition controller
		     * will now deal with this.
		     */
		}

		realCoordinator = null;
	    }
	    else
		_valid = false;
	}

	return _valid;
    }

public String type ()
    {
	return "/Resources/Arjuna/OTS_ServerTopLevelAction/OTS_ServerStrictTopLevelAction";
    }

private boolean _registered;
    
};
