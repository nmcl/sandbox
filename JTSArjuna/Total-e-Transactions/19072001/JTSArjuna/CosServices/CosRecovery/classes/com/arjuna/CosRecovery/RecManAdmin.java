/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: RecManAdmin.javatmpl,v 1.1.2.2.22.1.28.1 2001/06/15 12:08:00 nmcl Exp $
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
































































package com.arjuna.CosRecovery;

import com.arjuna.ArjunaCommon.Common.*;

import org.omg.CORBA.SystemException;

/**
 * Implementation of the RecoveryManager admin IDL interface
 * <P>
 * @author Dave Ingham (dave@arjuna.com)
 * @version $Id: RecManAdmin.javatmpl,v 1.1.2.2.22.1.28.1 2001/06/15 12:08:00 nmcl Exp $
 */

public class RecManAdmin extends  com.arjuna.CosRecovery.Admin . RecoveryManagerAdminPOA 
{
    public RecManAdmin(java.lang.String name)
    {


	if (DebugController.enabled()) 
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_CRASH_RECOVERY,
						 "RecManAdmin("+name+") created");
	}
    }

    /** Construct a transient object. */
    public RecManAdmin() 
    {
	super();

	if (DebugController.enabled()) 
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_CRASH_RECOVERY,
						 "RecManAdmin() created");
	}
    }

    /**
	A simple method to see if the RecoveryManager is awake.
       <p>
       Operation: <b>::Admin::RecoveryManagerAdmin::ping</b>.
       <pre>
       #pragma prefix "CosRecovery.arjuna.com/Admin/RecoveryManagerAdmin"
       long ping();
       </pre>
       </p>
       @return just a count of how many times it has been called
    */
    
    public int ping () throws SystemException
    {	
	int retVal = _pingCount++;
	
	if (DebugController.enabled()) 
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_CRASH_RECOVERY,
						 "RecManAdmin.ping() = "+retVal);
	}
	return retVal;
    }

    private static int _pingCount = 0;
}

