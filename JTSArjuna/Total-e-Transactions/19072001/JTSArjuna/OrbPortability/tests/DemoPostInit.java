/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: DemoPostInit.java,v 1.1 2000/02/25 14:10:01 nmcl Exp $
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
