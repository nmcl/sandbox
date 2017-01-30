/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DemoPostInit.java,v 1.1 1998/07/06 13:44:37 nmcl Exp $
 */

import com.arjuna.OrbCommon.*;

public class DemoPostInit
{

public DemoPostInit ()
    {
	System.out.println("DemoPostInit called.");

	if (ORBInterface.orb() != null)
	    System.out.println("The orb has been initialised.");
	else
	    System.out.println("Error - the orb has not been initialised.");
    }

};
