/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: DemoPreInit.java,v 1.1 2000/02/25 14:10:02 nmcl Exp $
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
