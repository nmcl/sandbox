/*
 * Copyright (C) 1998, 1999,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LocalSetup.java,v 1.1.6.1 1999/06/23 13:54:05 nmcl Exp $
 */

package com.arjuna.OrbCommon;

class LocalSetup extends ORBInit
{
    
public void initialise (String[] params)
    {
	for (int i = 0; i < params.length; i++)
	{
	    if (params[i].startsWith(LocalSetup.prefix))
	    {
		String propertyName = params[i].substring(LocalSetup.prefix.length());
		boolean setProperty = false;
		
		if ((propertyName.compareTo(environment.INITIAL_REFERENCES_ROOT) == 0) ||
		    (propertyName.compareTo(environment.INITIAL_REFERENCES_FILE) == 0) ||
		    (propertyName.compareTo(environment.FILE_DIR) == 0))
		{
		    setProperty = true;
		}

		if (setProperty)
		{
		    ORBPropertyManager.setProperty(propertyName, params[i+1]);
		    i++;
		}

		if (propertyName.compareTo(environment.CORBA_DIAGNOSTICS) == 0)
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
			ORBDebugController.setDebugLevel(level.intValue());

			level = null;
			toUse = null;
		    }
		    catch (Exception e)
		    {
			System.err.println("Invalid debug option "+params[i+1]);
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
