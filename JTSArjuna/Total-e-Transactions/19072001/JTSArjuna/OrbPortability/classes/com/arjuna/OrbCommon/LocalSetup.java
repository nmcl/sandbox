/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: LocalSetup.java,v 1.1.2.1.6.2.2.1.18.1 2000/11/23 15:17:37 nmcl Exp $
 */

package com.arjuna.OrbCommon;

import com.arjuna.ArjunaCommon.Common.*;

class LocalSetup extends ORBAttribute
{
    
public void initialise (String[] params)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ORB_PORTABILITY, "LocalSetup.initialise(String[] params)");
	}

	if (params == null)
	    return;
	
	for (int i = 0; i < params.length; i++)
	{
	    if (DebugController.enabled())
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						     FacilityCode.FAC_ORB_PORTABILITY, "Searching "+params[i]);
	    }

	    if (params[i].startsWith(LocalSetup.prefix))
	    {
		String propertyName = params[i].substring(LocalSetup.prefix.length());
		
		if (DebugController.enabled())
		{
		    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
							 FacilityCode.FAC_ORB_PORTABILITY, "Got "+propertyName);
		}

		if (DebugController.enabled())
		{
		    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
							 FacilityCode.FAC_ORB_PORTABILITY, "Setting property: "+propertyName+" with value: "+params[i+1]);
		}

		PropertyManager.setProperty(propertyName, params[i+1]);
		i++;

		if (propertyName.compareTo(ORBEnvironment.CORBA_DIAGNOSTICS) == 0)
		{
		    try
		    {
			/*
			 * Strip out any 0x extension as Java can't handle it!
			 */

			String toUse = null;

			if (params[i+1].startsWith(LocalSetup.hexStart))
			    toUse = params[i+1].substring(LocalSetup.hexStart.length());
			else
			    toUse = params[i+1];

			Integer level = Integer.valueOf(toUse, 16);
			DebugController.controller().setDebugLevel(level.intValue());

			level = null;
			toUse = null;
		    }
		    catch (Exception e)
		    {
			ErrorStream.stream(ErrorStream.WARNING).println("Invalid debug option "+params[i+1]);
		    }
		    
		    i++;
		}
		
		propertyName = null;
	    }
	}
    }

public static final String prefix = "-ARJUNA_";

private static final String hexStart = "0x";

};
