/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DemoPreInit.java,v 1.1 1998/07/06 13:44:38 nmcl Exp $
 */

import com.arjuna.OrbCommon.*;

public class DemoPreInit
{

public DemoPreInit ()
    {
	System.out.println("DemoPreInit called.");

	if (ORBInterface.orb() == null)
	    System.out.println("The orb has not been initialised.");
	else
	    System.out.println("Error - the orb has been initialised.");
    }

};
