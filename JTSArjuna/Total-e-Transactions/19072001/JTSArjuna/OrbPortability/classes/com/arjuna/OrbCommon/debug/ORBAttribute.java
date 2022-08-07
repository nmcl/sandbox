/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ORBAttribute.java,v 1.1.2.2 2000/12/21 11:21:30 nmcl Exp $
 */

package com.arjuna.OrbCommon.debug;

import com.arjuna.ArjunaCommon.Common.*;

/**
 * This class prints out all of the parameters passed to it.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ORBAttribute.java,v 1.1.2.2 2000/12/21 11:21:30 nmcl Exp $
 * @since JTS 2.1.
 */

public class ORBAttribute extends com.arjuna.OrbCommon.ORBAttribute
{
    
public void initialise (String[] params)
    {
	if (params != null)
	{
	    if (DebugController.enabled())
	    {
		DebugController.controller().print(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						     FacilityCode.FAC_ORB_PORTABILITY, "ORBAttribute.initialise - parameters: ");
	    }

	    for (int i = 0; i < params.length; i++)
	    {
		if (DebugController.enabled())
		{
		    DebugController.controller().print(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						       FacilityCode.FAC_ORB_PORTABILITY, params[i]);
		}
	    }

	    if (DebugController.enabled())
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						     FacilityCode.FAC_ORB_PORTABILITY, "");
	    }
	}
		
    }

public boolean postORBInit ()
    {
	return true;
    }
 
}

