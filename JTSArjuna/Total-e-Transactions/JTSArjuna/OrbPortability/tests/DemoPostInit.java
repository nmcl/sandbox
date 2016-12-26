/*
 * Copyright (C) 1999-2001 by HP Bluestone Software, Inc. All rights Reserved.
 *
 * HP Arjuna Labs,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: DemoPostInit.java,v 1.1.88.1 2001/08/09 16:15:43 delsworthy Exp $
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
